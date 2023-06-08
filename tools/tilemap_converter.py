import os
from collections import namedtuple

from pytmx import pytmx
from PIL import Image
from datetime import datetime
import incremental_build as inc_build


class TilemapConverter:
    class TooManyMTilesException(Exception):
        def __init__(self, mtile_count):
            super().__init__(f"Too many MTiles: {mtile_count} (max 255)")
            self.mtile_count = mtile_count

    @staticmethod
    def convert(tmx_path: str):
        """Convert a single `*.tmx` tilemap"""

        tilemap_name = os.path.splitext(os.path.basename(tmx_path))[0]

        # incremental build
        if not TilemapConverter.__should_build(tmx_path, tilemap_name):
            return

        tiled_map = pytmx.TiledMap(tmx_path)
        bg_upper: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGUpper")
        bg_upper2: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGUpper2")
        bg_lower: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGLower")
        objects: pytmx.TiledObjectGroup = tiled_map.get_layer_by_name("Objects")

        # generate intermediate MTileset
        mtileset_name = f"mtileset_{tilemap_name.lower()}_bg_upper"
        gid_mtile_idx_bg_upper = TilemapConverter.__generate_mtileset(
            bg_upper, mtileset_name, tiled_map
        )
        mtileset_name = f"mtileset_{tilemap_name.lower()}_bg_upper2"
        gid_mtile_idx_bg_upper2 = TilemapConverter.__generate_mtileset(
            bg_upper2, mtileset_name, tiled_map
        )
        mtileset_name = f"mtileset_{tilemap_name.lower()}_bg_lower"
        gid_mtile_idx_bg_lower = TilemapConverter.__generate_mtileset(
            bg_lower, mtileset_name, tiled_map
        )

        # generate header file
        # TODO: include flip
        TilemapConverter.__generate_mtilemap_header(
            [gid_mtile_idx_bg_lower, gid_mtile_idx_bg_upper, gid_mtile_idx_bg_upper2],
            tilemap_name,
            tiled_map,
        )

    @staticmethod
    def __generate_mtileset(
        tile_layer: pytmx.TiledTileLayer, mtileset_name: str, tiled_map: pytmx.TiledMap
    ):
        mtileset_name = mtileset_name.lower()

        tileset_rects = {}
        for x, y, gid in tile_layer.iter_data():
            if gid != 0:
                img_path, img_rect, tile_flags = tiled_map.get_tile_image_by_gid(gid)
                if img_path not in tileset_rects:
                    tileset_rects[img_path] = set()
                tileset_rects[img_path].add((gid, img_rect))

        mtile_count = sum(len(v) for v in tileset_rects.values())
        if mtile_count >= 255:
            raise TilemapConverter.TooManyMTilesException(mtile_count)

        # Width is 32 at first, which results in
        # "dominant" transparent color ends up the first entry of the quantized palette.
        # This will then cropped to width=16 on saving.
        mtileset = Image.new("RGBA", (32, 16 * (mtile_count + 1)), color="#00FF0000")
        gid_mtile_idx_mapping = {}

        for tileset_path, gid_rects in tileset_rects.items():
            with Image.open(tileset_path) as tileset:
                for gid, rect in gid_rects:
                    mtile_idx = len(gid_mtile_idx_mapping) + 1
                    gid_mtile_idx_mapping[gid] = mtile_idx
                    x, y, w, h = rect
                    mtile = tileset.crop((x, y, x + w, y + h))
                    mtileset.paste(mtile, (0, h * mtile_idx))

        ut_build_gfx_root = f"build_ut/graphics/"
        os.makedirs(ut_build_gfx_root, exist_ok=True)

        # save mtileset & palette `.bmp`
        mtileset = mtileset.quantize(16)
        mtileset = mtileset.crop((0, 0, 16, mtileset.height))
        mtileset.save(os.path.join(ut_build_gfx_root, f"{mtileset_name}.bmp"))
        mtileset = mtileset.resize((8, 8))
        mtileset.save(os.path.join(ut_build_gfx_root, f"pal_{mtileset_name}.bmp"))
        mtileset.close()

        # write `.json` for butano
        mtileset_json_path = os.path.join(ut_build_gfx_root, f"{mtileset_name}.json")
        pal_mtileset_json_path = os.path.join(
            ut_build_gfx_root, f"pal_{mtileset_name}.json"
        )
        with open(mtileset_json_path, "w") as f:
            f.write(
                '{"type":"regular_bg_tiles","bpp_mode":"bpp_4","compression":"auto"}'
            )
        with open(pal_mtileset_json_path, "w") as f:
            f.write('{"type":"bg_palette","bpp_mode":"bpp_4","compression":"auto"}')

        return gid_mtile_idx_mapping

    @staticmethod
    def __generate_mtilemap_header(
        gid_mtile_idx_dicts: list, mtilemap_name: str, tiled_map: pytmx.TiledMap
    ):
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        include_path = f"build_ut/include"
        os.makedirs(include_path, exist_ok=True)

        mtilemap_header_filename = f"{mtilemap_name}.hpp"
        mtilemap_header_path = f"{include_path}/{mtilemap_header_filename}"

        bg_upper: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGUpper")
        bg_upper2: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGUpper2")
        bg_lower: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGLower")

        with open(mtilemap_header_path, "w") as output_header:
            output_header.write(
                f"// Auto-generated by `tool/tilemap_converter.py` on {timestamp}\n"
            )
            output_header.write(
                f"// DO NOT EDIT this file;  It will be overwritten on next build!\n"
            )
            output_header.write("\n")
            output_header.write('#include "mtile/MTilemap.hpp"' + "\n")
            output_header.write("\n")
            output_header.write(
                f'#include "bn_bg_palette_items_pal_mtileset_{mtilemap_name}_bg_lower.h"\n'
            )
            output_header.write(
                f'#include "bn_bg_palette_items_pal_mtileset_{mtilemap_name}_bg_upper.h"\n'
            )
            output_header.write(
                f'#include "bn_bg_palette_items_pal_mtileset_{mtilemap_name}_bg_upper2.h"\n'
            )
            output_header.write(
                f'#include "bn_regular_bg_tiles_items_mtileset_{mtilemap_name}_bg_lower.h"\n'
            )
            output_header.write(
                f'#include "bn_regular_bg_tiles_items_mtileset_{mtilemap_name}_bg_upper.h"\n'
            )
            output_header.write(
                f'#include "bn_regular_bg_tiles_items_mtileset_{mtilemap_name}_bg_upper2.h"\n\n'
            )

            output_header.write(f"namespace ut::mtile" + "\n")
            output_header.write("{" + "\n\n")

            output_header.write(
                f"inline constexpr MTilemap<{tiled_map.width},{tiled_map.height}> {mtilemap_name}(\n"
            )
            output_header.write(
                f"bn::regular_bg_tiles_items::mtileset_{mtilemap_name}_bg_lower,\n"
            )
            output_header.write(
                f"bn::regular_bg_tiles_items::mtileset_{mtilemap_name}_bg_upper,\n"
            )
            output_header.write(
                f"bn::regular_bg_tiles_items::mtileset_{mtilemap_name}_bg_upper2,\n"
            )
            output_header.write(
                f"bn::bg_palette_items::pal_mtileset_{mtilemap_name}_bg_lower,\n"
            )
            output_header.write(
                f"bn::bg_palette_items::pal_mtileset_{mtilemap_name}_bg_upper,\n"
            )
            output_header.write(
                f"bn::bg_palette_items::pal_mtileset_{mtilemap_name}_bg_upper2,\n"
            )

            BoardType = namedtuple("BoardType", "mTileIdx, hFlip, vFlip")

            def write_board(bg: pytmx.TiledTileLayer, gid_mtile_idx_mapping: dict):
                board = (
                    [BoardType(0, False, False)] * tiled_map.width * tiled_map.height
                )
                for x, y, gid in bg.iter_data():
                    if gid != 0:
                        tile_flags: pytmx.TileFlags = tiled_map.get_tile_image_by_gid(
                            gid
                        )[-1]
                        board[y * tiled_map.width + x] = BoardType(
                            gid_mtile_idx_mapping[gid],
                            tile_flags.flipped_horizontally,
                            tile_flags.flipped_vertically,
                        )

                output_header.write("{" + "\n")
                board_str = "".join(
                    f"{{{cell.mTileIdx},{1 if cell.hFlip else 0},{1 if cell.vFlip else 0}}},"
                    for cell in board
                )
                if board_str:
                    output_header.write("MTile")
                    output_header.write(board_str)
                output_header.write("}" + "\n")

            write_board(bg_lower, gid_mtile_idx_dicts[0])
            output_header.write(",")
            write_board(bg_upper, gid_mtile_idx_dicts[1])
            output_header.write(",")
            write_board(bg_upper2, gid_mtile_idx_dicts[2])
            output_header.write(");" + "\n\n")

            output_header.write("} // namespace ut::mtile" + "\n")

    @staticmethod
    def __should_build(tmx_path: str, tilemap_name: str) -> bool:
        tmx_path_split = os.path.normpath(tmx_path).split(os.sep)
        tilemap_folder = tmx_path_split[-2]

        tileset_png_path = (
            f"extra/tilemaps/{tilemap_folder}/{tilemap_folder}_tileset.png"
        )
        tileset_tsx_path = f"extra/tilemaps/{tilemap_folder}/{tilemap_folder}.tsx"
        bg_lower_build_path = (
            f"build_ut/graphics/mtileset_{tilemap_name.lower()}_bg_lower.bmp"
        )

        return (
            inc_build.should_build(tileset_png_path, bg_lower_build_path)
            or inc_build.should_build(tileset_tsx_path, bg_lower_build_path)
            or inc_build.should_build(tmx_path, bg_lower_build_path)
        )

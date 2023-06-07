import os
from collections import namedtuple

from pytmx import pytmx
from PIL import Image


class TilemapConverter:
    class TooManyMTilesException(Exception):
        def __init__(self, mtile_count):
            super().__init__(f"Too many MTiles: {mtile_count} (max 255)")
            self.mtile_count = mtile_count

    @staticmethod
    def convert(tmx_path: str):
        """Convert a single `*.tmx` tilemap"""
        tiled_map = pytmx.TiledMap(tmx_path)
        tilemap_name = os.path.splitext(os.path.basename(tmx_path))[0]
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
        # mtileset_name = f"mtileset_{tilemap_name.lower()}_bg_lower"
        # gid_mtile_idx_bg_lower = TilemapConverter.__generate_mtileset(
        #     bg_lower, mtileset_name, tiled_map
        # )

        # TODO: generate header file (including board, flip...)

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

        mtileset = Image.new("RGBA", (16, 16 * (mtile_count + 1)))
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
            f.write('{"type":"regular_bg_tiles","bpp_mode":"bpp_4"}')
        with open(pal_mtileset_json_path, "w") as f:
            f.write('{"type":"bg_palette","bpp_mode":"bpp_4"}')

        return gid_mtile_idx_mapping

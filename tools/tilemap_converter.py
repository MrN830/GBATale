import os
import glob
import json
from collections import namedtuple
from enum import Enum
from collections import Counter

from pytmx import pytmx
from PIL import Image
from timestamp_writer import write_timestamp
import incremental_build as inc_build


class TilemapConverter:
    class TooManyMTilesException(Exception):
        def __init__(self, mtile_count):
            super().__init__(f"Too many MTiles: {mtile_count} (max 255)")
            self.mtile_count = mtile_count

    class InvalidWallColliderException(Exception):
        def __init__(self, coll_pos):
            super().__init__(f"Invalid collider: {coll_pos}")
            self.coll_pos = coll_pos

    @staticmethod
    def write_roominfo_cpp():
        RoomKind = Enum("RoomKind", TilemapConverter.ROOM_NAMES, start=0)
        room_header_exists = [False] * len(RoomKind)
        for room_header in glob.glob(f"build_ut/include/gen/room_*.hpp"):
            room_name = os.path.splitext(os.path.basename(room_header))[0]
            room_header_exists[eval(f"RoomKind.{room_name.upper()}.value")] = True

        os.makedirs("build_ut/src", exist_ok=True)
        with open("build_ut/src/RoomInfoGen.cpp", "w") as cpp_file:
            write_timestamp(cpp_file, "tool/tilemap_converter.py")

            cpp_file.write('#include "game/RoomInfo.hpp"' + "\n\n")
            cpp_file.write("#include <bn_assert.h>" + "\n\n")
            cpp_file.write('#include "mtile/MTilemap.hpp"' + "\n\n")

            for room_header in glob.glob(f"build_ut/include/gen/room_*.hpp"):
                cpp_file.write(f'#include "gen/{os.path.basename(room_header)}"' + "\n")
            cpp_file.write("\n")

            cpp_file.write("namespace ut::game {" + "\n\n")
            cpp_file.write(
                "static constexpr const mtile::MTilemapBase* ROOMS[(int)RoomKind::TOTAL_COUNT] = {\n"
            )

            for i, room_kind in enumerate(RoomKind):
                mtilemap_str = (
                    f"&mtile::gen::{str(room_kind).split('.')[-1].lower()},"
                    if room_header_exists[i]
                    else "nullptr,"
                )
                cpp_file.write(mtilemap_str + "\n")

            cpp_file.write("};" + "\n\n")

            cpp_file.write(
                "auto getRoomMTilemap(RoomKind kind) -> const mtile::MTilemapBase* {\n"
            )
            cpp_file.write(
                "BN_ASSERT((int)RoomKind::NONE <= (int)kind && (int)kind < (int)RoomKind::TOTAL_COUNT);\n\n"
            )
            cpp_file.write("if (kind == RoomKind::NONE) return nullptr;" + "\n\n")
            cpp_file.write("return ROOMS[(int)kind];" + "\n")
            cpp_file.write("}" + "\n")

            cpp_file.write("} // namespace ut::game" + "\n")

    @staticmethod
    def write_entity_id_header():
        include_path = f"build_ut/include/gen"
        os.makedirs(include_path, exist_ok=True)

        entity_ids = {"NONE", "frisk", "flowey"}

        for folder in glob.glob("extra/tilemaps/*"):
            if not os.path.isdir(folder):
                continue

            for tmx_path in glob.glob(f"{folder}/*.tmx"):
                tiled_map = pytmx.TiledMap()
                tiled_map.parse_json(
                    json.load(open("extra/tilemaps/GBATale.tiled-project", "r"))[
                        "propertyTypes"
                    ]
                )
                tiled_map.filename = tmx_path
                tiled_map.parse_xml(
                    pytmx.ElementTree.parse(tiled_map.filename).getroot()
                )

                l_entity: pytmx.TiledObjectGroup = tiled_map.get_layer_by_name("Entity")

                for obj in l_entity:
                    if obj.name:
                        entity_ids.add(obj.name)

        header_filename = f"EntityId.hpp"
        header_path = f"{include_path}/{header_filename}"

        with open(header_path, "w") as output_header:
            write_timestamp(output_header, "tool/tilemap_converter.py")
            output_header.write("#pragma once" + "\n")
            output_header.write("#include <cstdint>" + "\n")

            output_header.write("namespace ut::game::ent::gen {" + "\n")
            output_header.write("enum class EntityId : uint16_t {" + "\n")
            for entity_id in entity_ids:
                output_header.write(f"{entity_id}," + "\n")
            output_header.write("};" + "\n")
            output_header.write("} // namespace ut::game::ent::gen" + "\n")

    @staticmethod
    def write_path_sources():
        include_path = f"build_ut/include/gen"
        src_path = f"build_ut/src"
        os.makedirs(include_path, exist_ok=True)
        os.makedirs(src_path, exist_ok=True)

        paths_dict = {}

        for folder in glob.glob("extra/tilemaps/*"):
            if not os.path.isdir(folder):
                continue

            for tmx_path in glob.glob(f"{folder}/*.tmx"):
                tiled_map = pytmx.TiledMap()
                tiled_map.parse_json(
                    json.load(open("extra/tilemaps/GBATale.tiled-project", "r"))[
                        "propertyTypes"
                    ]
                )
                tiled_map.filename = tmx_path
                tiled_map.parse_xml(
                    pytmx.ElementTree.parse(tiled_map.filename).getroot()
                )

                l_path: pytmx.TiledObjectGroup = tiled_map.get_layer_by_name("Path")

                for obj in l_path:
                    if obj.type != "Path":
                        raise Exception(
                            f"{obj.type=} found in layer `Path` in `{tmx_path}` (x={obj.x}, y={obj.y})"
                        )
                    if not obj.name:
                        raise Exception(
                            f"unnamed path found in `{tmx_path}` (x={obj.x}, y={obj.y})"
                        )

                    path = paths_dict[obj.name] = obj.apply_transformations()

                    if len(path) <= 1:
                        raise Exception(
                            f"path with {len(path)} point found in `{tmx_path}` (x={obj.x}, y={obj.y})"
                        )

            header_filename = f"PathId.hpp"
            source_filename = f"Paths.cpp"
            header_path = f"{include_path}/{header_filename}"
            source_path = f"{src_path}/{source_filename}"

            with open(header_path, "w") as output_header:
                write_timestamp(output_header, "tool/tilemap_converter.py")
                output_header.write("#pragma once" + "\n")

                output_header.write("namespace ut::asset::gen {" + "\n")
                output_header.write("enum class PathId {" + "\n")
                for path_id in paths_dict:
                    output_header.write(f"{path_id}," + "\n")
                output_header.write("TOTAL_COUNT" + "\n")
                output_header.write("};" + "\n")
                output_header.write("} // namespace ut::asset::gen" + "\n")

            with open(source_path, "w") as output_source:
                write_timestamp(output_source, "tool/tilemap_converter.py")
                output_source.write('#include "asset/Path.hpp"' + "\n")
                output_source.write('#include "gen/PathId.hpp"' + "\n")

                output_source.write("namespace ut::asset {" + "\n")

                output_source.write("namespace {" + "\n")

                for path_id, points in paths_dict.items():
                    output_source.write(
                        f"constexpr const Path<{len(points)}> {path_id}(" + "\n"
                    )
                    output_source.write(
                        f"bn::array<bn::fixed_point, {len(points)}>{{" + "\n"
                    )
                    output_source.write(
                        "".join(f"bn::fixed_point{p}," for p in points) + "\n"
                    )
                    output_source.write("}" + "\n")
                    output_source.write(");" + "\n")

                output_source.write(
                    f"constexpr const IPath* PATHS[(int)gen::PathId::TOTAL_COUNT] = {{"
                    + "\n"
                )
                output_source.write(
                    "".join(f"&{path_id}," for path_id in paths_dict) + "\n"
                )
                output_source.write("};" + "\n")

                output_source.write("} // namespace" + "\n")

                output_source.write(
                    "auto IPath::get(gen::PathId id) -> const IPath& {" + "\n"
                )
                output_source.write(
                    'BN_ASSERT(0 <= (int)id && (int)id < (int)gen::PathId::TOTAL_COUNT, "Invalid PathId=", (int)id);'
                    + "\n"
                )
                output_source.write("return *PATHS[(int)id];" + "\n")
                output_source.write("}" + "\n")
                output_source.write("} // namespace ut::asset" + "\n")

    def write_colls_header(self):
        with open("build_ut/include/gen/StaticCollInfos.hpp") as output_header:
            write_timestamp(output_header, "tool/tilemap_converter.py")
            output_header.write("#pragma once" + "\n")
            output_header.write('#include "game/coll/CollInfo.hpp"' + "\n")
            output_header.write("#include <bn_array.h>" + "\n")

            output_header.write("namespace ut::game::coll::gen {" + "\n")

            for i, colls in enumerate(self.static_coll_infos):
                output_header.write(
                    f"inline constexpr const bn::array<RectCollInfo,{len(colls)}> _staticCollInfos{i}"
                    + "{\n"
                )
                output_header.write(
                    "".join(
                        f"RectCollInfo(bn::fixed_point({coll.x},{coll.y}),bn::fixed_size({coll.w},{coll.h})),"
                        for coll in colls
                    )
                )
                output_header.write("};\n")

            output_header.write("} // namespace ut::game::coll::gen" + "\n")

    def convert(self, tmx_path: str) -> bool:
        """Convert a single `*.tmx` tilemap"""

        tilemap_name = os.path.splitext(os.path.basename(tmx_path))[0]

        # incremental build
        if not TilemapConverter.__should_build(tmx_path, tilemap_name):
            return False

        tiled_map = pytmx.TiledMap()
        tiled_map.parse_json(
            json.load(open("extra/tilemaps/GBATale.tiled-project", "r"))[
                "propertyTypes"
            ]
        )
        tiled_map.filename = tmx_path
        tiled_map.parse_xml(pytmx.ElementTree.parse(tiled_map.filename).getroot())

        bg_upper2: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGUpper2")
        bg_upper: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGUpper")
        bg_lower: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGLower")

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
        try:
            self.__generate_mtilemap_header(
                [
                    gid_mtile_idx_bg_lower,
                    gid_mtile_idx_bg_upper,
                    gid_mtile_idx_bg_upper2,
                ],
                tilemap_name.lower(),
                tiled_map,
            )
        except:
            mtilemap_header_filename = f"{tilemap_name.lower()}.hpp"
            mtilemap_header_path = f"build_ut/include/gen/{mtilemap_header_filename}"
            if os.path.exists(mtilemap_header_path):
                os.remove(mtilemap_header_path)
            raise

        return True

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

    def __generate_mtilemap_header(
        self, gid_mtile_idx_dicts: list, mtilemap_name: str, tiled_map: pytmx.TiledMap
    ):
        include_path = f"build_ut/include/gen"
        os.makedirs(include_path, exist_ok=True)

        mtilemap_header_filename = f"{mtilemap_name}.hpp"
        mtilemap_header_path = f"{include_path}/{mtilemap_header_filename}"

        l_entity: pytmx.TiledObjectGroup = tiled_map.get_layer_by_name("Entity")
        l_warp: pytmx.TiledObjectGroup = tiled_map.get_layer_by_name("Warp")
        l_wall: pytmx.TiledObjectGroup = tiled_map.get_layer_by_name("Wall")
        bg_upper: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGUpper")
        bg_upper2: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGUpper2")
        bg_lower: pytmx.TiledTileLayer = tiled_map.get_layer_by_name("BGLower")

        # Walls
        RectWall = namedtuple("RectWall", "x, y, w, h")
        TriWall = namedtuple("TriWall", "x, y, l, direc")

        def get_rect_wall(points: list) -> RectWall:
            if (
                len(points) != 4
                or points[0].x != points[1].x
                or points[1].y != points[2].y
                or points[2].x != points[3].x
                or points[3].y != points[0].y
            ):
                return None

            p1, p2 = points[0], points[2]
            rect_wall = RectWall(
                (p1.x + p2.x) / 2,
                (p1.y + p2.y) / 2,
                abs(p2.x - p1.x),
                abs(p2.y - p1.y),
            )
            if rect_wall.w == 0 or rect_wall.h == 0:
                return None
            return rect_wall

        def get_tri_wall(points: list) -> TriWall:
            if (
                len(points) != 3
                or len(set(points)) != 3
                or len({p.x for p in points}) != 2
                or len({p.y for p in points}) != 2
            ):
                return None

            x_common = Counter(p.x for p in points).most_common(2)
            y_common = Counter(p.y for p in points).most_common(2)
            x, x_count = x_common[0]
            y, y_count = y_common[0]
            assert x_count == 2 and y_count == 2
            other_x, x_count = x_common[1]
            other_y, y_count = y_common[1]
            assert x_count == 1 and y_count == 1
            direc = f"(core::Directions::{ 'UP' if y < other_y else 'DOWN' }|core::Directions::{'LEFT' if x < other_x else 'RIGHT'})"
            l = abs(x - other_x)
            if l != abs(y - other_y):
                return None

            return TriWall(x, y, l, direc)

        rect_walls = []
        tri_walls = []
        for obj in l_wall:
            if obj.type:
                raise Exception(
                    f"{obj.type=} found in `{mtilemap_name}` (x={obj_pos.x}, y={obj_pos.y})"
                )

            if hasattr(obj, "points"):
                tri = get_tri_wall(obj.points)
                if tri:
                    tri_walls.append(tri)
                    continue
                else:
                    obj_pos = obj.as_points[0]
                    print(
                        f"[ERR] Invalid Wall collider found in `{mtilemap_name}` (x={obj_pos.x}, y={obj_pos.y})"
                    )
                    raise TilemapConverter.InvalidWallColliderException(obj_pos)

            rect = get_rect_wall(obj.as_points)
            if rect:
                rect_walls.append(rect)
                continue

            obj_pos = obj.as_points[0]
            print(
                f"[ERR] Invalid Wall collider found in `{mtilemap_name}` (x={obj_pos.x}, y={obj_pos.y})"
            )
            raise TilemapConverter.InvalidWallColliderException(obj_pos)

        # Entities
        ColliderPack = namedtuple("ColliderPack", "isEnabled, isTrigger, collInfos")
        Interaction = namedtuple("Interaction", "isEnabled, type, triggers")
        EventComponent = namedtuple("EventComponent", "isEnabled, type, isAutoFire")
        SpriteCpnt = namedtuple(
            "SpriteCpnt",
            "isEnabled, sprItem, gfxIdx, isBlendingEnabled, updateZOrderOnMove, zOrder, bgPriority",
        )
        SprAnimCpnt = namedtuple("SprAnimCpnt", "isEnabled, kind")
        WalkAnimCtrlCpnt = namedtuple("WalkAnimCtrlCpnt", "isEnabled, kind")
        NpcInput = namedtuple("NpcInput", "isEnabled")

        entities = []
        spr_items = set()
        interaction_cpp_classes = set()
        ev_cpnt_cpp_classes = set()

        for obj in l_entity:
            if obj.type != "Entity":
                raise Exception(
                    f"{obj.type=} found in `{mtilemap_name}` (x={obj.x}, y={obj.y})"
                )

            entity = {
                "id": obj.name if obj.name else "NONE",
                "x": obj.x if obj.image else obj.x + obj.width / 2,
                "y": obj.y + obj.height if obj.image else obj.y + obj.height,
            }
            no_component_entity_len = len(entity)

            def get_sprItem_gfxIdx():
                img_path = obj.image[0]
                img_name = os.path.splitext(os.path.basename(img_path))[0]
                img_json_path = os.path.splitext(img_path)[0] + ".json"
                img_height = json.load(open(img_json_path, "r"))["height"]
                tile_prop = tiled_map.get_tile_properties_by_gid(obj.gid)
                tile_y = tile_prop.get("y", 0.0)

                sprItem = img_name
                gfxIdx = int(tile_y // img_height)
                return sprItem, gfxIdx

            # cpnt::ColliderPack
            collPack = obj.properties.get("colliderPack")
            if collPack:

                def get_coll_infos(colls):
                    coll_infos = []
                    for coll in colls:
                        rect = get_rect_wall(coll.as_points)
                        if not rect:
                            raise Exception(
                                f"Invalid colliderPack found in `{mtilemap_name}` Entity layer (x={obj.x}, y={obj.y})"
                            )
                        coll_infos.append(rect)
                    return coll_infos

                # tile colliders
                if "colliders" in obj.properties:
                    colls = obj.properties["colliders"]
                    collInfos = get_coll_infos([coll for coll in colls])
                    collInfos = [
                        RectWall(
                            collInfo.x - obj.width / 2,
                            collInfo.y - obj.height,
                            collInfo.w,
                            collInfo.h,
                        )
                        for collInfo in collInfos
                    ]
                else:  # entity rect collider
                    collInfos = get_coll_infos([obj])
                    collInfos[0] = RectWall(
                        0,
                        -collInfos[0].h / 2,
                        collInfos[0].w,
                        collInfos[0].h,
                    )

                entity["colliderPack"] = ColliderPack(
                    collPack.isEnabled, collPack.isTrigger, collInfos
                )

            # child of `cpnt::inter::Interaction`
            interaction = obj.properties.get("interaction")
            if interaction:
                if not collPack:
                    raise Exception(
                        f"obj has `cpnt::inter::Interaction` but doesn't have `cpnt::ColliderPack` in `{mtilemap_name}` (x={obj.x}, y={obj.y})"
                    )
                if not interaction.triggers:
                    raise Exception(
                        f"obj has empty `interaction.triggers` in `{mtilemap_name}` (x={obj.x}, y={obj.y})"
                    )

                interaction_cpp_classes.add(interaction.InteractionCppClass)

                entity["interaction"] = Interaction(
                    interaction.isEnabled,
                    interaction.InteractionCppClass,
                    interaction.triggers.split(","),
                )

            # child of `cpnt::ev::EventComponent`
            ev_cpnt = obj.properties.get("eventComponent")
            if ev_cpnt:
                ev_cpnt_cpp_classes.add(ev_cpnt.EventComponentCppClass)

                entity["eventComponent"] = EventComponent(
                    ev_cpnt.isEnabled,
                    ev_cpnt.EventComponentCppClass,
                    ev_cpnt.isAutoFire,
                )

            # cpnt::Sprite
            spr = obj.properties.get("sprite")
            if spr:
                if not obj.image:
                    raise Exception(
                        f"obj has cpnt::Sprite but doesn't have an image in `{mtilemap_name}` (x={obj.x}, y={obj.y})"
                    )
                if not (-32768 <= spr.zOrder <= 32767):
                    raise Exception(
                        f"invalid sprite.zOrder in `{mtilemap_name}` (x={obj.x}, y={obj.y})"
                    )
                if not (0 <= spr.bgPriority <= 3):
                    raise Exception(
                        f"invalid sprite.bgPriority in `{mtilemap_name}` (x={obj.x}, y={obj.y})"
                    )

                sprItem, gfxIdx = get_sprItem_gfxIdx()

                entity["sprite"] = SpriteCpnt(
                    spr.isEnabled,
                    sprItem,
                    gfxIdx,
                    spr.isBlendingEnabled,
                    spr.updateZOrderOnMove,
                    spr.zOrder,
                    spr.bgPriority,
                )
                spr_items.add(sprItem)

            # cpnt::SpriteAnim
            sprAnim = obj.properties.get("spriteAnim")
            if sprAnim:
                if not spr:
                    raise Exception(
                        f"obj has cpnt::SpriteAnim but doesn't have cpnt::Sprite in `{mtilemap_name}` (x={obj.x}, y={obj.y})"
                    )

                entity["sprAnim"] = SprAnimCpnt(sprAnim.isEnabled, sprAnim.kind)

            # cpnt::WalkAnimCtrl
            walk = obj.properties.get("walkAnimCtrl")
            if walk:
                if not sprAnim:
                    raise Exception(
                        f"obj has cpnt::WalkAnimCtrl but doesn't have cpnt::SpriteAnim in `{mtilemap_name}` (x={obj.x}, y={obj.y})"
                    )

                entity["walkAnimCtrl"] = WalkAnimCtrlCpnt(walk.isEnabled, walk.kind)

            # cpnt::NpcInput
            npcInput = obj.properties.get("npcInput")
            if npcInput:
                entity["npcInput"] = NpcInput(npcInput.isEnabled)

            # Add entity
            if len(entity) <= no_component_entity_len:
                print(
                    f"[WARN] entity with NO component found in `{mtilemap_name}` (x={obj_pos.x}, y={obj_pos.y})"
                )
            entities.append(entity)

        # Warps & warp points
        Warp = namedtuple("Warp", "rect, roomName, warpId, isBgmFadeOut")
        WarpPoint = namedtuple("WarpPoint", "x, y")
        warp_ids = ["INIT", "A", "B", "C", "D", "X"]

        warps = []
        warp_points = [None] * len(warp_ids)

        for obj in l_warp:
            if obj.type not in {"Warp", "WarpPoint"}:
                raise Exception(
                    f"Non warp obj found in `{mtilemap_name}` Warp layer (x={obj.x}, y={obj.y})"
                )

            warpId = obj.properties.get("warpId")
            if not warpId or warpId not in warp_ids:
                raise Exception(
                    f"Invalid {warpId=} found in `{mtilemap_name}` Warp layer (x={obj.x}, y={obj.y})"
                )

            if obj.type == "Warp":
                roomName = obj.properties.get("roomName")
                if (
                    not roomName
                    or roomName.upper() not in TilemapConverter.ROOM_NAMES_SET
                ):
                    raise Exception(
                        f"Invalid {roomName=} found in `{mtilemap_name}` Warp layer (x={obj.x}, y={obj.y})"
                    )
                roomName = roomName.upper()

                rect = get_rect_wall(obj.as_points)
                if not rect:
                    raise Exception(
                        f"Invalid sized warp found in `{mtilemap_name}` Warp layer (x={obj.x}, y={obj.y})"
                    )

                isBgmFadeOut = obj.properties.get("isBgmFadeOut", False)

                warps.append(Warp(rect, roomName, warpId, isBgmFadeOut))

            else:  # obj.type == "WarpPoint"
                idx = warp_ids.index(warpId)

                if warp_points[idx]:
                    raise Exception(
                        f"Duplicate {warpId=} for WarpPoints (x={obj.x}, y={obj.y}) & (x={warp_points[idx].x}, y={warp_points[idx].y}) in `{mtilemap_name}` Warp layer"
                    )

                warp_points[idx] = WarpPoint(obj.x, obj.y)

        # Write header file
        with open(mtilemap_header_path, "w") as output_header:
            write_timestamp(output_header, "tool/tilemap_converter.py")

            output_header.write("#pragma once" + "\n\n")

            output_header.write('#include "mtile/MTilemap.hpp"' + "\n\n")

            output_header.write('#include "asset/SpriteAnimKind.hpp"' + "\n")
            output_header.write('#include "gen/EntityId.hpp"' + "\n\n")

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
                f'#include "bn_regular_bg_tiles_items_mtileset_{mtilemap_name}_bg_upper2.h"\n'
            )

            for spr_item in spr_items:
                output_header.write(f'#include "bn_sprite_items_{spr_item}.h"\n')
            output_header.write("\n")

            for inter_type in interaction_cpp_classes:
                output_header.write(f'#include "game/cpnt/inter/{inter_type}.hpp"\n')
            for ev_cpnt_type in ev_cpnt_cpp_classes:
                output_header.write(f'#include "game/cpnt/ev/{ev_cpnt_type}.hpp"\n')
            output_header.write("\n")

            output_header.write(f"namespace ut::mtile::gen" + "\n")
            output_header.write("{" + "\n\n")

            output_header.write(f"using namespace ut::game::coll;" + "\n")
            output_header.write(f"using namespace ut::game::ent;" + "\n\n")

            # arrays vary in size

            # coll infos
            for i, entity in enumerate(entities):
                if "colliderPack" in entity:
                    collPack: ColliderPack = entity["colliderPack"]
                    output_header.write(
                        f"inline constexpr bn::array<RectCollInfo, {len(collPack.collInfos)}> _{mtilemap_name}_collInfos_{i}"
                        + "{\n"
                    )
                    output_header.write(
                        "".join(
                            f"RectCollInfo(bn::fixed_point({coll.x},{coll.y}),bn::fixed_size({coll.w},{coll.h})),"
                            for coll in collPack.collInfos
                        )
                    )
                    output_header.write("};\n")

            # entities
            output_header.write(
                f"inline constexpr bn::array<EntityInfo,{len(entities)}> _{mtilemap_name}_entities"
                + "{\n"
            )

            for i, entity in enumerate(entities):
                output_header.write("EntityInfo{")
                output_header.write(f"game::ent::gen::EntityId::{entity['id']},")
                output_header.write(f"bn::fixed_point({entity['x']},{entity['y']}),")

                # cpnt::ColliderPack
                if "colliderPack" in entity:
                    collPack: ColliderPack = entity["colliderPack"]
                    output_header.write(
                        f"EntityInfo::ColliderPack(_{mtilemap_name}_collInfos_{i},{str(collPack.isEnabled).lower()},{str(collPack.isTrigger).lower()}),"
                    )
                else:
                    output_header.write("bn::nullopt,")

                # child of `cpnt::inter::Interaction`
                if "interaction" in entity:
                    interaction: Interaction = entity["interaction"]
                    output_header.write(
                        f"EntityInfo::Interaction(bn::type_id<game::cpnt::inter::{interaction.type}>(), ({'|'.join(f'game::cpnt::inter::InteractionTriggers::{tr}' for tr in interaction.triggers)}), {str(interaction.isEnabled).lower()}),"
                    )
                else:
                    output_header.write("bn::nullopt,")

                # child of `cpnt::ev::EventComponent`
                if "eventComponent" in entity:
                    ev_cpnt: EventComponent = entity["eventComponent"]
                    output_header.write(
                        f"EntityInfo::EventComponent(bn::type_id<game::cpnt::ev::{ev_cpnt.type}>(),{str(ev_cpnt.isEnabled).lower()},{str(ev_cpnt.isAutoFire).lower()}),"
                    )
                else:
                    output_header.write("bn::nullopt,")

                # cpnt::Sprite
                if "sprite" in entity:
                    spr: SpriteCpnt = entity["sprite"]
                    output_header.write(
                        f"EntityInfo::Sprite(bn::sprite_items::{spr.sprItem},{spr.gfxIdx},{spr.zOrder},{spr.bgPriority},{str(spr.isEnabled).lower()},{str(spr.isBlendingEnabled).lower()},{str(spr.updateZOrderOnMove).lower()}),"
                    )
                else:
                    output_header.write("bn::nullopt,")

                # cpnt::SpriteAnim
                if "sprAnim" in entity:
                    sprAnim: SprAnimCpnt = entity["sprAnim"]
                    output_header.write(
                        f"EntityInfo::SpriteAnim(asset::SpriteAnimKind::{sprAnim.kind},{str(sprAnim.isEnabled).lower()}),"
                    )
                else:
                    output_header.write("bn::nullopt,")

                # cpnt::WalkAnimCtrl
                if "walkAnimCtrl" in entity:
                    walk: WalkAnimCtrlCpnt = entity["walkAnimCtrl"]
                    output_header.write(
                        f"EntityInfo::WalkAnimCtrl(asset::WalkAnimKind::{walk.kind},{str(walk.isEnabled).lower()}),"
                    )
                else:
                    output_header.write("bn::nullopt,")

                # cpnt::NpcInput
                if "npcInput" in entity:
                    npcInput: NpcInput = entity["npcInput"]
                    output_header.write(
                        f"EntityInfo::NpcInput({str(npcInput.isEnabled).lower()}),"
                    )
                else:
                    output_header.write("bn::nullopt,")

                output_header.write("},")

            output_header.write("};" + "\n\n")

            ### MTilemap ###
            output_header.write(
                f"inline constexpr MTilemap<{tiled_map.width},{tiled_map.height},{len(entities)},{len(rect_walls)},{len(tri_walls)},{len(warps)}> {mtilemap_name}(\n"
            )

            # MTileset
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

            # Entities
            output_header.write(f"_{mtilemap_name}_entities,\n")

            # Rect walls
            output_header.write("{" + "\n")
            output_header.write(
                "".join(
                    f"RectCollInfo(bn::fixed_point({wall.x},{wall.y}),bn::fixed_size({wall.w},{wall.h})),"
                    for wall in rect_walls
                )
            )
            output_header.write("}," + "\n")

            # Tri walls
            output_header.write("{" + "\n")
            output_header.write(
                "".join(
                    f"AAIRTriCollInfo(bn::fixed_point({wall.x},{wall.y}),{wall.l},{wall.direc}),"
                    for wall in tri_walls
                )
            )
            output_header.write("}," + "\n")

            # Warps
            output_header.write("{" + "\n")
            output_header.write(
                "".join(
                    f"Warp(RectCollInfo(bn::fixed_point({warp.rect.x},{warp.rect.y}),bn::fixed_size({warp.rect.w},{warp.rect.h})),game::RoomKind::{warp.roomName},WarpId::{warp.warpId},{str(warp.isBgmFadeOut).lower()}),"
                    for warp in warps
                )
            )
            output_header.write("}," + "\n")

            # Warp points
            output_header.write("{" + "\n")
            output_header.write(
                "".join(
                    f"bn::fixed_point({f'{wp.x},{wp.y}' if wp else '-999,-999'}),"
                    for wp in warp_points
                )
            )
            output_header.write("}," + "\n")

            # Board
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

            output_header.write("} // namespace ut::mtile::gen" + "\n")

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

        mtilemap_header_filename = f"{tilemap_name.lower()}.hpp"
        mtilemap_header_path = f"build_ut/include/gen/{mtilemap_header_filename}"

        return (
            inc_build.should_build("tools/tilemap_converter.py", mtilemap_header_path)
            or inc_build.should_build(tileset_png_path, bg_lower_build_path)
            or inc_build.should_build(tileset_tsx_path, bg_lower_build_path)
            or inc_build.should_build(tmx_path, bg_lower_build_path)
            or inc_build.should_build(tmx_path, mtilemap_header_path)
        )

    ROOM_NAMES = [
        "ROOM_START",
        "ROOM_INTROSTORY",
        "ROOM_INTROIMAGE",
        "ROOM_INTROMENU",
        "ROOM_AREA1",
        "ROOM_AREA1_2",
        "ROOM_RUINS1",
        "ROOM_RUINS2",
        "ROOM_RUINS3",
        "ROOM_RUINS4",
        "ROOM_RUINS5",
        "ROOM_RUINS6",
        "ROOM_RUINS7",
        "ROOM_RUINS7A",
        "ROOM_RUINS8",
        "ROOM_RUINS9",
        "ROOM_RUINS10",
        "ROOM_RUINS11",
        "ROOM_RUINS12A",
        "ROOM_RUINS12",
        "ROOM_RUINS12B",
        "ROOM_RUINS13",
        "ROOM_RUINS14",
        "ROOM_RUINS15A",
        "ROOM_RUINS15B",
        "ROOM_RUINS15C",
        "ROOM_RUINS15D",
        "ROOM_RUINS15E",
        "ROOM_RUINS16",
        "ROOM_RUINS17",
        "ROOM_RUINS18OLD",
        "ROOM_RUINS19",
        "ROOM_TORHOUSE1",
        "ROOM_TORHOUSE2",
        "ROOM_TORHOUSE3",
        "ROOM_TORIELROOM",
        "ROOM_ASRIELROOM",
        "ROOM_KITCHEN",
        "ROOM_BASEMENT1",
        "ROOM_BASEMENT2",
        "ROOM_BASEMENT3",
        "ROOM_BASEMENT4",
        "ROOM_BASEMENT5",
        "ROOM_RUINSEXIT",
        "ROOM_TUNDRA1",
        "ROOM_TUNDRA2",
        "ROOM_TUNDRA3",
        "ROOM_TUNDRA3A",
        "ROOM_TUNDRA4",
        "ROOM_TUNDRA5",
        "ROOM_TUNDRA6",
        "ROOM_TUNDRA6A",
        "ROOM_TUNDRA7",
        "ROOM_TUNDRA8",
        "ROOM_TUNDRA8A",
        "ROOM_TUNDRA9",
        "ROOM_TUNDRA_SPAGHETTI",
        "ROOM_TUNDRA_SNOWPUZZ",
        "ROOM_TUNDRA_XOXOSMALL",
        "ROOM_TUNDRA_XOXOPUZZ",
        "ROOM_TUNDRA_RANDOBLOCK",
        "ROOM_TUNDRA_LESSERDOG",
        "ROOM_TUNDRA_ICEHOLE",
        "ROOM_TUNDRA_ICEENTRANCE",
        "ROOM_TUNDRA_ICEEXIT_NEW",
        "ROOM_TUNDRA_ICEEXIT",
        "ROOM_TUNDRA_POFFZONE",
        "ROOM_TUNDRA_DANGERBRIDGE",
        "ROOM_TUNDRA_TOWN",
        "ROOM_TUNDRA_TOWN2",
        "ROOM_TUNDRA_DOCK",
        "ROOM_TUNDRA_INN",
        "ROOM_TUNDRA_INN_2F",
        "ROOM_TUNDRA_GRILLBY",
        "ROOM_TUNDRA_LIBRARY",
        "ROOM_TUNDRA_GARAGE",
        "ROOM_TUNDRA_SANSHOUSE",
        "ROOM_TUNDRA_PAPROOM",
        "ROOM_TUNDRA_SANSROOM",
        "ROOM_TUNDRA_SANSROOM_DARK",
        "ROOM_TUNDRA_SANSBASEMENT",
        "ROOM_FOGROOM",
        "ROOM_WATER1",
        "ROOM_WATER2",
        "ROOM_WATER3",
        "ROOM_WATER3A",
        "ROOM_WATER4",
        "ROOM_WATER_BRIDGEPUZZ1",
        "ROOM_WATER5",
        "ROOM_WATER5A",
        "ROOM_WATER6",
        "ROOM_WATER7",
        "ROOM_WATER8",
        "ROOM_WATER9",
        "ROOM_WATER_SAVEPOINT1",
        "ROOM_WATER11",
        "ROOM_WATER_NICECREAM",
        "ROOM_WATER12",
        "ROOM_WATER_SHOE",
        "ROOM_WATER_BIRD",
        "ROOM_WATER_ONIONSAN",
        "ROOM_WATER14",
        "ROOM_WATER_PIANO",
        "ROOM_WATER_DOGROOM",
        "ROOM_WATER_STATUE",
        "ROOM_WATER_PREWATERFALL",
        "ROOM_WATER_WATERFALL",
        "ROOM_WATER_WATERFALL2",
        "ROOM_WATER_WATERFALL3",
        "ROOM_WATER_WATERFALL4",
        "ROOM_WATER_PREUNDYNE",
        "ROOM_WATER_UNDYNEBRIDGE",
        "ROOM_WATER_UNDYNEBRIDGEEND",
        "ROOM_WATER_TRASHZONE1",
        "ROOM_WATER_TRASHSAVEPOINT",
        "ROOM_WATER_TRASHZONE2",
        "ROOM_WATER_FRIENDLYHUB",
        "ROOM_WATER_UNDYNEYARD",
        "ROOM_WATER_UNDYNEHOUSE",
        "ROOM_WATER_BLOOKYARD",
        "ROOM_WATER_BLOOKHOUSE",
        "ROOM_WATER_HAPSTABLOOK",
        "ROOM_WATER_FARM",
        "ROOM_WATER_PREBIRD",
        "ROOM_WATER_SHOP",
        "ROOM_WATER_DOCK",
        "ROOM_WATER15",
        "ROOM_WATER16",
        "ROOM_WATER_TEMVILLAGE",
        "ROOM_WATER17",
        "ROOM_WATER18",
        "ROOM_WATER19",
        "ROOM_WATER20",
        "ROOM_WATER21",
        "ROOM_WATER_UNDYNEFINAL",
        "ROOM_WATER_UNDYNEFINAL2",
        "ROOM_WATER_UNDYNEFINAL3",
        "ROOM_FIRE1",
        "ROOM_FIRE2",
        "ROOM_FIRE_PRELAB",
        "ROOM_FIRE_DOCK",
        "ROOM_FIRE_LAB1",
        "ROOM_FIRE_LAB2",
        "ROOM_FIRE3",
        "ROOM_FIRE5",
        "ROOM_FIRE6",
        "ROOM_FIRE6A",
        "ROOM_FIRE_LASERS1",
        "ROOM_FIRE7",
        "ROOM_FIRE8",
        "ROOM_FIRE_SHOOTGUY_2",
        "ROOM_FIRE9",
        "ROOM_FIRE_SHOOTGUY_1",
        "ROOM_FIRE_TURN",
        "ROOM_FIRE_COOKINGSHOW",
        "ROOM_FIRE_SAVEPOINT1",
        "ROOM_FIRE_ELEVATOR_R1",
        "ROOM_FIRE_ELEVATOR_R2",
        "ROOM_FIRE_HOTDOG",
        "ROOM_FIRE_WALKANDBRANCH",
        "ROOM_FIRE_SORRY",
        "ROOM_FIRE_APRON",
        "ROOM_FIRE10",
        "ROOM_FIRE_RPUZZLE",
        "ROOM_FIRE_MEWMEW2",
        "ROOM_FIRE_BOYSNIGHTOUT",
        "ROOM_FIRE_NEWSREPORT",
        "ROOM_FIRE_COREVIEW2",
        "ROOM_FIRE_ELEVATOR_L2",
        "ROOM_FIRE_ELEVATOR_L3",
        "ROOM_FIRE_SPIDERSHOP",
        "ROOM_FIRE_WALKANDBRANCH2",
        "ROOM_FIRE_CONVEYORLASER",
        "ROOM_FIRE_SHOOTGUY_3",
        "ROOM_FIRE_PRESHOOTGUY4",
        "ROOM_FIRE_SHOOTGUY_4",
        "ROOM_FIRE_SAVEPOINT2",
        "ROOM_FIRE_SPIDER",
        "ROOM_FIRE_PACING",
        "ROOM_FIRE_OPERATEST",
        "ROOM_FIRE_MULTITILE",
        "ROOM_FIRE_HOTELFRONT_1",
        "ROOM_FIRE_HOTELFRONT_2",
        "ROOM_FIRE_HOTELLOBBY",
        "ROOM_FIRE_RESTAURANT",
        "ROOM_FIRE_HOTELDOORS",
        "ROOM_FIRE_HOTELBED",
        "ROOM_FIRE_ELEVATOR_R3",
        "ROOM_FIRE_PRECORE",
        "ROOM_FIRE_CORE1",
        "ROOM_FIRE_CORE2",
        "ROOM_FIRE_CORE3",
        "ROOM_FIRE_CORE4",
        "ROOM_FIRE_CORE5",
        "ROOM_FIRE_CORE_FREEBATTLE",
        "ROOM_FIRE_CORE_LASERFUN",
        "ROOM_FIRE_CORE_BRANCH",
        "ROOM_FIRE_CORE_BOTTOMLEFT",
        "ROOM_FIRE_CORE_LEFT",
        "ROOM_FIRE_CORE_TOPLEFT",
        "ROOM_FIRE_CORE_TOP",
        "ROOM_FIRE_CORE_TOPRIGHT",
        "ROOM_FIRE_CORE_RIGHT",
        "ROOM_FIRE_CORE_BOTTOMRIGHT",
        "ROOM_FIRE_CORE_CENTER",
        "ROOM_FIRE_SHOOTGUY_5",
        "ROOM_FIRE_CORE_TREASURELEFT",
        "ROOM_FIRE_CORE_TREASURERIGHT",
        "ROOM_FIRE_CORE_WARRIOR",
        "ROOM_FIRE_CORE_BRIDGE",
        "ROOM_FIRE_CORE_PREMETT",
        "ROOM_FIRE_CORE_METTTEST",
        "ROOM_FIRE_CORE_FINAL",
        "ROOM_FIRE_ELEVATOR",
        "ROOM_FIRE_ELEVATOR_L1",
        "ROOM_FIRE_FINALELEVATOR",
        "ROOM_CASTLE_ELEVATOROUT",
        "ROOM_CASTLE_PRECASTLE",
        "ROOM_CASTLE_HOOK",
        "ROOM_CASTLE_FRONT",
        "ROOM_ASGHOUSE1",
        "ROOM_ASGHOUSE2",
        "ROOM_ASGHOUSE3",
        "ROOM_ASGOREROOM",
        "ROOM_ASRIELROOM_FINAL",
        "ROOM_KITCHEN_FINAL",
        "ROOM_BASEMENT1_FINAL",
        "ROOM_BASEMENT2_FINAL",
        "ROOM_BASEMENT3_FINAL",
        "ROOM_BASEMENT4_FINAL",
        "ROOM_LASTRUINS_CORRIDOR",
        "ROOM_SANSCORRIDOR",
        "ROOM_CASTLE_FINALSHOEHORN",
        "ROOM_CASTLE_COFFINS1",
        "ROOM_CASTLE_COFFINS2",
        "ROOM_CASTLE_THRONEROOM",
        "ROOM_CASTLE_PREBARRIER",
        "ROOM_CASTLE_BARRIER",
        "ROOM_CASTLE_EXIT",
        "ROOM_UNDERTALE_END",
        "ROOM_CASTLE_TRUEEXIT",
        "ROOM_OUTSIDEWORLD",
        "ROOM_FIRE_LABELEVATOR",
        "ROOM_TRUELAB_ELEVATORINSIDE",
        "ROOM_TRUELAB_ELEVATOR",
        "ROOM_TRUELAB_HALL1",
        "ROOM_TRUELAB_HUB",
        "ROOM_TRUELAB_HALL2",
        "ROOM_TRUELAB_OPERATINGROOM",
        "ROOM_TRUELAB_REDLEVER",
        "ROOM_TRUELAB_PREBED",
        "ROOM_TRUELAB_BEDROOM",
        "ROOM_TRUELAB_MIRROR",
        "ROOM_TRUELAB_BLUELEVER",
        "ROOM_TRUELAB_HALL3",
        "ROOM_TRUELAB_SHOWER",
        "ROOM_TRUELAB_DETERMINATION",
        "ROOM_TRUELAB_TV",
        "ROOM_TRUELAB_COOLER",
        "ROOM_TRUELAB_GREENLEVER",
        "ROOM_TRUELAB_FAN",
        "ROOM_TRUELAB_CASTLE_ELEVATOR",
        "ROOM_TRUELAB_PREPOWER",
        "ROOM_TRUELAB_POWER",
        "ROOM_GASTER",
        "ROOM_ICECAVE1",
        "ROOM_ICE_DOG",
        "ROOM2",
        "ROOM_WATER_FAKEHALLWAY",
        "ROOM_MYSTERYMAN",
        "ROOM_SOUNDTEST",
        "TESTROOM",
        "ROOM_WATER_REDACTED",
        "ROOM_WATER13",
        "ROOM_OVERWORLD",
        "ROOM_OVERWORLD3",
        "BULLETTEST",
        "ROOM_WATER16A",
        "ROOM_END_CASTROLL",
        "ROOM_END_HIGHWAY",
        "ROOM_END_BEACH",
        "ROOM_END_METTA",
        "ROOM_END_SCHOOL",
        "ROOM_END_MTEBOTT",
        "ROOM_CREDITSDODGER",
        "ROOM_END_MYROOM",
        "ROOM_END_THEEND",
        "ROOM_SPRITECHECK",
        "ROOM_JOYCONFIG",
        "ROOM_CONTROLTEST",
        "ROOM_F_START",
        "ROOM_F_INTRO",
        "ROOM_F_MENU",
        "ROOM_F_ROOM",
        "ROOM_FLOWEYX",
        "ROOM_F_PHRASE",
        "ROOM_FIRE4",
        "ROOM_FIRE10_OLD",
        "ROOM_FIRE10A_OLD",
        "ROOM_TUNDRA_PLACEHOLDER",
        "ROOM_RUINS12B_OLD",
        "ROOM_TUNDRA_ROLLSNOW",
        "ROOM_WATER7_OLDER",
        "ROOM_MEETUNDYNE_OLD",
        "ROOM_WATER_MUSHROOM",
        "ROOM_MONSTERALIGN_TEST",
        "ROOM_BATTLE",
        "ROOM_FLOWEYBATTLE",
        "ROOM_FASTBATTLE",
        "ROOM_STORYBATTLE",
        "ROOM_GAMEOVER",
        "ROOM_SHOP1",
        "ROOM_SHOP2",
        "ROOM_SHOP3",
        "ROOM_SHOP4",
        "ROOM_SHOP5",
        "ROOM_RIVERMAN_TRANSITION",
        "ROOM_PAPDATE",
        "ROOM_ADATE",
        "ROOM_FLOWEY_ENDCHOICE",
        "ROOM_FLOWEY_REGRET",
        "ROOM_EMPTY",
        "ROOM_EMPTYWHITE",
        "ROOM_EMPTYBLACK",
        "ROOM_NOTHINGNESS",
        "ROOM_UNDERTALE",
        "ROOM_OF_DOG",
        "ROOM_QUIZHOLDER",
        "ROOM_FRIENDTEST",
        "ROOM_BRINGITINGUYS",
        "ROOM_ASRIELAPPEARS",
        "ROOM_GOODBYEASRIEL",
        "ROOM_ASRIELMEMORY",
        "ROOM_ASRIELTEST",
        "ROOM_AFINALTEST",
    ]

    ROOM_NAMES_SET = set(ROOM_NAMES)

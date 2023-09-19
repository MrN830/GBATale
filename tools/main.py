import os
import glob

from tilemap_converter import TilemapConverter
from textdata_dumper import dump_and_convert_textdata

if __name__ == "__main__":
    print("`tools/main.py`: Running...")

    dump_and_convert_textdata()

    # Tilemap conversion
    conv = TilemapConverter()
    converted = False
    for folder in glob.glob("extra/tilemaps/*"):
        if os.path.isdir(folder):
            for file in glob.glob(f"{folder}/*.tmx"):
                converted = conv.convert(file) or converted

    if converted:
        TilemapConverter.write_roominfo_cpp()
        TilemapConverter.write_entity_id_header()
        TilemapConverter.write_path_sources()

    print("`tools/main.py`: Done!")

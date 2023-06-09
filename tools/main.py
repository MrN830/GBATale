import os
import glob

from tilemap_converter import TilemapConverter

if __name__ == "__main__":
    print("`tools/main.py`: Running...")

    # Tilemap conversion
    converted = False
    for folder in glob.glob("extra/tilemaps/*"):
        if os.path.isdir(folder):
            for file in glob.glob(f"{folder}/*.tmx"):
                converted = TilemapConverter.convert(file) or converted

    if converted:
        TilemapConverter.write_roominfo_cpp()

    print("`tools/main.py`: Done!")

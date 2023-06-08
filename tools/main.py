import os
import glob

from tilemap_converter import TilemapConverter

if __name__ == "__main__":
    # Tilemap conversion
    for folder in glob.glob("extra/tilemaps/*"):
        if os.path.isdir(folder):
            for file in glob.glob(f"{folder}/*.tmx"):
                TilemapConverter.convert(file)

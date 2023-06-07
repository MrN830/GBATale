import os
import argparse

from tilemap_converter import TilemapConverter

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Entry point of asset pre-conversion scripts."
    )
    parser.add_argument("--build", required=True, help="User build path")
    args = parser.parse_args()

    # Test
    TilemapConverter.convert("extra/tilemaps/Ruins.xml")

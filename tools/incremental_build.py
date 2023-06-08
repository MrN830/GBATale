import os


def should_build(source_item_path: str, built_item_path: str) -> bool:
    if not os.path.exists(built_item_path):
        return True

    source_mtime = os.path.getmtime(source_item_path)
    built_mtime = os.path.getmtime(built_item_path)
    return source_mtime > built_mtime

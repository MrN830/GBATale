import os
import json
from subprocess import Popen

import incremental_build as inc_build
from timestamp_writer import write_timestamp

json_path = "user_deps.json"

def dump_and_convert_textdata():
    deps = __parse_user_deps()

    data_win_path = deps["UndertaleDataWinPath"]
    mod_cli_path = deps["UndertaleModCliPath"]

    if not inc_build.should_build(data_win_path, "build_ut/src/TextData.cpp"):
        return

    print(f'{data_win_path=}, {mod_cli_path=}')

    # Dump `gml_Script_textdata_en.gml`
    try:
        process = Popen([mod_cli_path, "dump", "--code", "gml_Script_textdata_en", "--output", "build_ut/dump", data_win_path])
        exit_code = process.wait()
        if exit_code != 0:
            raise Exception(f"UndertaleModCli terminated with exit code {exit_code}")
    except FileNotFoundError as e:
        print("Either `UndertaleDataWinPath` or `UndertaleModCliPath` is invalid")
        raise

    # Deserialize GML to python list
    text_datas = list()
    ds_map_add = lambda a, b: (a, b)

    with open("build_ut/dump/CodeEntries/gml_Script_textdata_en.gml") as gml_file:
        for line in gml_file:
            if "ds_map_add" in line:
                line = line.replace('global.text_data_en, ', '')
                text_datas.append(eval(line))

    # Generate header file
    os.makedirs("build_ut/include/gen", exist_ok=True)
    with open("build_ut/include/gen/TextData.hpp", "w") as header_file:
        write_timestamp(header_file, "tools/textdata_dumper.py")

        header_file.write("#pragma once" + "\n\n")
        
        header_file.write("#include <cstdint>" + "\n\n")

        header_file.write("namespace bn { class string_view; }" + "\n\n")

        header_file.write("namespace ut::asset::gen {" + "\n\n")

        header_file.write("enum class TextData : int16_t;" + "\n\n")

        header_file.write("auto getTextEn(TextData) -> const bn::string_view&;" + "\n\n")

        header_file.write("enum class TextData : int16_t {" + "\n")
        header_file.write("NONE = -1," + "\n")
        for k, v in text_datas:
            header_file.write(f"{k}," + "\n")
        header_file.write("TOTAL_COUNT" + "\n")
        header_file.write("};" + "\n\n")
        
        header_file.write("} // namespace ut::asset::gen" + "\n")

    # Generate source file
    os.makedirs("build_ut/src", exist_ok=True)
    with open("build_ut/src/TextData.cpp", "w") as cpp_file:
        write_timestamp(cpp_file, "tools/textdata_dumper.py")

        cpp_file.write('#include "gen/TextData.hpp"' + "\n\n")

        cpp_file.write("#include <bn_assert.h>" + "\n")
        cpp_file.write("#include <bn_string_view.h>" + "\n\n")

        cpp_file.write("namespace ut::asset::gen {" + "\n\n")

        cpp_file.write("static constexpr const bn::string_view TEXT_DATA_EN[(int)TextData::TOTAL_COUNT] = {" + "\n")
        for k, v in text_datas:
            cpp_file.write(f'R"({v})",' + "\n")
        cpp_file.write("};" + "\n\n")

        cpp_file.write("auto getTextEn(TextData data) -> const bn::string_view& {" + "\n")
        cpp_file.write("BN_ASSERT(0 <= (int)data && (int)data < (int)TextData::TOTAL_COUNT);" + "\n")
        cpp_file.write("return TEXT_DATA_EN[(int)data];" + "\n")
        cpp_file.write("}" + "\n\n")
        
        cpp_file.write("} // namespace ut::asset::gen" + "\n")
        

def __parse_user_deps() -> dict:
    deps = {"UndertaleDataWinPath":"", "UndertaleModCliPath":""}

    if not os.path.exists(json_path):
        # Generate empty deps json
        with open(json_path, "w") as f:
            json.dump(deps, f)

        print("To build GBATale, you have to specify some user dependencies path.")
        print(f"Edit the generated `{json_path}` to specify the dependency paths.")
        raise Exception(f"empty {json_path}")

    with open(json_path, 'r') as f:
        deps: dict = json.loads(f.read())

        if "UndertaleDataWinPath" not in deps or not deps["UndertaleDataWinPath"].strip():
            raise Exception(f"key `UndertaleDataWinPath` not found or empty in `{json_path}`")
        if "UndertaleModCliPath" not in deps or not deps["UndertaleModCliPath"].strip():
            raise Exception(f"key `UndertaleModCliPath` not found or empty in `{json_path}`")

    return deps

# GBATale

Unofficial Undertale port for the Game Boy Advance.

## Build

1. Setup dependencies, and try to build it first.
    ```bash
    git submodule update --init
    pip install -r requirements.txt

    make -j{num_of_cpu_cores}
    ```
2. It'll fail to build first, and create `user_deps.json` on the root.\
   Download the dependencies manually, and specify the path to them on the `user_deps.json`.
   * "UndertaleDataWinPath"
      * Extract `data.win` from your copy of Undertale, specify the path to it.
   * "[UndertaleModCli](https://github.com/krzys-h/UndertaleModTool/releases/tag/bleeding-edge)Path"
      * Download the version starts with `CLI-*`, specify the path to the executable.
3. You should now be able to build GBATale with `make -j{num_of_cpu_cores}`.


End of GBATale description, Butano template readme below.

-----

# Butano template

GBA gamedev template for [Butano engine](https://github.com/GValiente/butano).

This template is meant to be used on [VSCode](https://code.visualstudio.com/), with [Native Debug](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) extension.


## Changing paths

You need to change some paths in this template so that they suit your environment.

1. Change `LIBBUTANO :=` on [Makefile](Makefile#L30) to your location for Butano.


## Finding include paths with C/C++ extension

[See this gist.](https://gist.github.com/copyrat90/eee49d92846ca3585a69d5bea001710d)


## Setting up debugger

[launch.json](launch.json) and [tasks.json](tasks.json) are provided to easily debug your game with [mGBA](https://mgba.io/).

1. Add `mgba` and `devkitARM/bin/arm-none-eabi-gdb` executables to your `PATH`.
2. Install [Native Debug extension](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) on VSCode.
3. Settings done; Hit F5 on VSCode to debug.

Optionally, add `-Og` to `USERFLAGS` on Makefile to improve debugging.\
Don't forget to remove `-Og` before your actual release!

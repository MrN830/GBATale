# GBATale

Unofficial Undertale port for the Game Boy Advance.


## Build

0. [Setup all the dependencies of Butano engine](https://gvaliente.github.io/butano/getting_started.html).
1. Clone this repo recursively, install Python3 requirements, and try to build it first.
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


## Setup dev environment

Install [VSCode](https://code.visualstudio.com/) and [Native Debug](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) extension.


### Find include paths with C/C++ extension (IntelliSense)

[See this gist.](https://gist.github.com/copyrat90/eee49d92846ca3585a69d5bea001710d)


### Setup mGBA debugging

[See this guide.](https://felixjones.co.uk/mgba_gdb/vscode.html)

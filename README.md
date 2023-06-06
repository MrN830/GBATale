# GBATale

Unofficial Undertale port for the Game Boy Advance.

## Build

```bash
git submodule update --init
pip install -r requirements.txt

make -j{num_of_cpu_cores}
```


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

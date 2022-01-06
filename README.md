# petraea
ARM9 Emulator

Named after the [Sessile Oak](https://en.wikipedia.org/wiki/Quercus_petraea), which is hard for a casual observer to differentiate from the more common English Oak. Hopefully it will be equally difficult to tell whether software is running on this emulator, or the more common Acorn-designed processor.

Uses criterion for unit testing: https://github.com/Snaipe/Criterion

## How to compile
Petraea uses meson as a build system, you may need to install this.

Run the following commands (parts in square brackets are optional).

```sh
meson build/ [-Dprefix=/path/to/install] && cd build/
meson compile
meson test
meson install
```


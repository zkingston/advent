# Advent of Code

Solutions to [Advent of Code](https://adventofcode.com/), written in C.
Beyond solving the both of the puzzles, the secondary objective is to have the solver be as fast as possible, including parsing (aiming for <1ms).

Compile with `meson`:
```bash
meson build
meson compile -C build
```

After adding a new day:
```bash
meson setup --reconfigure build
```

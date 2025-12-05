# Advent of Code

Solutions to [Advent of Code](https://adventofcode.com/), written in C, with some additional constraints:
- solve both of the puzzles in a single file with minimal duplicated code,
- be as fast as possible, including parsing (aiming for <1ms),
- fit in as few lines as possible while still maintaining style

Compile with `meson`:
```bash
meson build
meson compile -C build
```

After adding a new day:
```bash
meson setup --reconfigure build
```

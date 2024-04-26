# Particles

Particles is a very simple particle system built in C, compiled to `wasm` with Raylib.

## Build

Requirements:

-   Raylib
-   Emscripten

Steps:

1. Compile Raylib. Place generated library files into `lib/`.
2. Run `emmake make`.
3. Run `emrun ./build/particles.html` to run the executable.

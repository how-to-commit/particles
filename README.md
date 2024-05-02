# Particles

Particles is a very simple particle system built in C, compiled to `wasm` with Raylib.

It's not meant to be very realistic, just a fancy distraction. Balls gain speed when they bump into walls to keep them moving.

## Build

Requirements:

-   Raylib
-   Emscripten

Steps:

1. Compile Raylib. Place generated static library files and header files into `lib/`.
2. Run `make run`.

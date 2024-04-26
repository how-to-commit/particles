#include <stdio.h>

#include "emscripten/emscripten.h"
#include <raylib.h>

#include <particle.h>
#include <point2f.h>

const int screenWidth = 800;
const int screenHeight = 400;

Point2f pos, vel, acc;
Particle particle;
double currTime, prevTime, deltaTime;

const int targetFps = 60;

void init() {
    printf("[particles] Init called...\n");

    // create positioning data
    p2f_init(&pos, 100, screenHeight / 2);
    p2f_init(&vel, 0, 0);
    p2f_init(&acc, 10, 0);

    // create particle
    particle_init(&particle, -1, &pos, &vel, &acc, false);

    // set time
    prevTime = 0.0;
    currTime = 0.0;
    deltaTime = 0.0;
}

void calculate_delta_time() {
    currTime = GetTime();

    float drawTime = currTime - prevTime;
    float waitTime = (1.0 / (float)targetFps) - drawTime;

    if (waitTime > 0.0) {
        WaitTime(waitTime);
        currTime = GetTime();
        deltaTime = currTime - prevTime;
    } else {
        deltaTime = drawTime;
    }
    prevTime = currTime;
}

void update() {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("Test", 100, 100, 20, LIGHTGRAY);

    if (IsKeyPressed(KEY_SPACE))
        init();

    DrawCircle(particle.position.x, particle.position.y, 20, RED);
    printf("[particles] Drawn at %f %f, dT %f\n", particle.position.x, particle.position.y, deltaTime);
    particle_simulate(&particle, deltaTime);

    EndDrawing();
    calculate_delta_time();
}

int main() {
    init();

    InitWindow(screenWidth, screenHeight, "test");
    SetTargetFPS(targetFps);

    emscripten_set_main_loop(update, 0, 1);
}

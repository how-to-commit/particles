#include "emscripten/emscripten.h"
#include <raylib.h>

#include <particle.h>
#include <point2f.h>

const int screenWidth = 800;
const int screenHeight = 600;

const Point2f initialPos = {100, screenHeight / 2};
const Point2f initialVel = {100, 100};
const Point2f initialAcc = {0, 0};

PSystem psys;
double currTime, prevTime, deltaTime;

const int targetFps = 60;

void init() {
    // create particle system
    psys = psys_init();
    psys_add(
        &psys,
        (Particle){.ttl = -1, .mass = 20, .position = initialPos, .velocity = initialVel, .acceleration = initialAcc});

    // set time
    prevTime = 0.0;
    currTime = 0.0;
    deltaTime = 0.0;
}

void calculate_delta_time() {
    currTime = (double)GetTime();

    double drawTime = currTime - prevTime;
    double waitTime = (1.0 / (double)targetFps) - drawTime;

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

    // if (IsKeyPressed(KEY_SPACE))
    //     reset();

    size_t sysSize = sizeof(psys.particles) / sizeof(Particle);

    for (size_t i = 0; i < sysSize; i++) {
        if (psys.particles[i].ttl != 0) {
            particle_simulate(&psys.particles[i], deltaTime);
            DrawCircle((int)psys.particles[i].position.x, (int)psys.particles[i].position.y, psys.particles[i].mass,
                       RED);
        }
    }

    EndDrawing();
    calculate_delta_time();
}

int main() {
    init();

    InitWindow(screenWidth, screenHeight, "test");

    emscripten_set_main_loop(update, targetFps, 0);
}

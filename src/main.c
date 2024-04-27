#include "emscripten/emscripten.h"
#include <raylib.h>

#include <particle.h>
#include <point2f.h>

const int screenWidth = 800;
const int screenHeight = 400;

const Point2f initialPos = {100, screenHeight / 2};
const Point2f initialVel = {0, 0};
const Point2f initialAcc = {10, 0};

Point2f pos, vel, acc;
Particle particle;
double currTime, prevTime, deltaTime;

const int targetFps = 60;

void init() {
    // create positioning data
    pos = initialPos;
    vel = initialVel;
    acc = initialAcc;

    // create particle
    particle_init(&particle, 300, &pos, &vel, &acc, false);

    // set time
    prevTime = 0.0;
    currTime = 0.0;
    deltaTime = 0.0;
}

void reset() {
    particle.position = initialPos;
    particle.velocity = initialVel;
    particle.acceleration = initialAcc;
    particle.isDead = false;
    particle.ttl = 300;
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
        reset();

    DrawCircle(particle.position.x, particle.position.y, 20, RED);
    particle_simulate(&particle, deltaTime);

    EndDrawing();
    calculate_delta_time();
}

int main() {
    init();

    InitWindow(screenWidth, screenHeight, "test");

    emscripten_set_main_loop(update, targetFps, 0);
}

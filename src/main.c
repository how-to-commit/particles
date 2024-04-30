#include "emscripten/emscripten.h"
#include <raylib.h>

#include <particle.h>
#include <point2f.h>

const int screenWidth = 800;
const int screenHeight = 600;

PSystem psys;
double currTime, prevTime, deltaTime;

const int targetFps = 60;

void init() {
    // create particle system
    // (0, 1000)px/s^2 accel gravity
    psys = psys_init((Point2f){0, 1000});

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

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 _mPosVec2 = GetMousePosition();
        Point2f mousePos = {_mPosVec2.x, _mPosVec2.y};

        psys_add(
            &psys,
            (Particle){
                .ttl = -1, .mass = 20, .position_current = mousePos, .position_old = mousePos, .acceleration = {0}});
    }
    psys_simulate(&psys, deltaTime);

    // draw step
    ClearBackground(RAYWHITE);
    DrawText("Test", 100, 100, 20, LIGHTGRAY);

    // draw particles - only draw alive ones
    size_t sysSize = sizeof(psys.particles) / sizeof(Particle);
    for (size_t i = 0; i < sysSize; i++) {
        if (psys.particles[i].ttl != 0) {
            DrawCircle((int)psys.particles[i].position_current.x, (int)psys.particles[i].position_current.y,
                       psys.particles[i].mass, RED);
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

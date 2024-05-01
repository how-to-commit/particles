#include "emscripten/emscripten.h"
#include <raylib.h>

#include <particle.h>
#include <point2f.h>

int screenWidth, screenHeight;

PSystem psys;
double currTime, prevTime, deltaTime;

const int targetFps = 60;
const Color ballColours[] = {YELLOW, ORANGE, PINK, RED, MAROON};

EM_JS(int, get_css_screen_width, (), { return window.innerWidth; });
EM_JS(int, get_css_screen_height, (), { return window.innerHeight; });

void init() {
    screenWidth = get_css_screen_width();
    screenHeight = get_css_screen_height();

    // try to fill the whole page
    InitWindow(screenWidth, screenHeight, "Particles");

    // create particle system
    // (0, 300)px/s^2 accel gravity
    psys = psys_init((Point2f){0, 300});

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

Color color_particles(Particle *p) {
    // color based on the size of the particle
    return ballColours[((p->mass - 5) / 4) - 1];
}

void update() {
    BeginDrawing();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 _mPosVec2 = GetMousePosition();
        Point2f mousePos = {_mPosVec2.x, _mPosVec2.y};

        // generate random initial velocity in a random direction
        Point2f oldPos = {_mPosVec2.x + GetRandomValue(-30, 30), _mPosVec2.y + GetRandomValue(-30, 30)};

        // generate random size
        int mass = GetRandomValue(5, 25);

        psys_add(
            &psys,
            (Particle){
                .ttl = -1, .mass = mass, .position_current = mousePos, .position_old = oldPos, .acceleration = {0}});
    }

    psys_simulate(&psys, deltaTime);

    // draw step
    ClearBackground(RAYWHITE);
    DrawText("Left click to spawn particles.", 100, 100, 20, LIGHTGRAY);

    // draw particles - only draw alive ones
    size_t sysSize = sizeof(psys.particles) / sizeof(Particle);
    for (size_t i = 0; i < sysSize; i++) {
        if (psys.particles[i].ttl != 0) {
            DrawCircle((int)psys.particles[i].position_current.x, (int)psys.particles[i].position_current.y,
                       psys.particles[i].mass, color_particles(&psys.particles[i]));
        }
    }

    EndDrawing();
    calculate_delta_time();
}

int main() {
    init();
    emscripten_set_main_loop(update, targetFps, 0);
}

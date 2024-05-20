#include "emscripten/emscripten.h"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <particle.h>
#include <point2f.h>

int screenWidth, screenHeight;

PSystem psys;
double currTime, prevTime, deltaTime;
Particle cursor;

float accel_x = 0.0;
float accel_y = 300.0;
float timescale = 1.0;

const int VERLET_SUBSTEPS = 5;
const int TARGET_FPS = 60;
const int INPUT_VARIANCE = 10;
const Color BALL_COLORS[] = {YELLOW, ORANGE, PINK, RED, MAROON};

EM_JS(int, get_css_screen_width, (), { return window.innerWidth; });
EM_JS(int, get_css_screen_height, (), { return window.innerHeight; });

void init() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    screenWidth = get_css_screen_width();
    screenHeight = get_css_screen_height();

    // try to fill the whole page
    InitWindow(screenWidth, screenHeight, "Particles");

    // create particle system
    psys = psys_init((Point2f){accel_x, accel_y});
    cursor = (Particle){.acceleration = (Point2f){0, 0},
                        .mass = 30,
                        .position_current = (Point2f){0, 0},
                        .position_old = (Point2f){0, 0},
                        .ttl = -1};

    // set time
    prevTime = 0.0;
    currTime = 0.0;
    deltaTime = 0.0;
}

void calculate_delta_time() {
    currTime = (double)GetTime();

    double drawTime = currTime - prevTime;
    double waitTime = (1.0 / (double)TARGET_FPS) - drawTime;

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
    return BALL_COLORS[((p->mass - 5) / 4) - 1];
}

void update() {
    BeginDrawing();
    Vector2 mpos_vec2 = GetMousePosition();
    Point2f mousePos = {mpos_vec2.x, mpos_vec2.y};

    // conf simulation
    psys.sys_accel = (Point2f){accel_x, accel_y};
    cursor.position_old = cursor.position_current;
    cursor.position_current = mousePos;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (mpos_vec2.x > 280 || mpos_vec2.y < 190 || mpos_vec2.y > 290) {
            // generate random initial velocity in a random direction
            Point2f oldPos = {mpos_vec2.x + GetRandomValue(-INPUT_VARIANCE, INPUT_VARIANCE),
                              mpos_vec2.y + GetRandomValue(-INPUT_VARIANCE, INPUT_VARIANCE)};

            // generate random size
            int mass = GetRandomValue(5, 25);

            psys_add(&psys, (Particle){.ttl = -1,
                                       .mass = mass,
                                       .position_current = mousePos,
                                       .position_old = oldPos,
                                       .acceleration = {0}});
        }
    } else {
        collide_with_cursor(&psys, &cursor);
    }

    // simulate
    for (int i = 0; i < VERLET_SUBSTEPS; i++)
        psys_simulate(&psys, deltaTime * timescale / VERLET_SUBSTEPS);

    // draw step
    // text at background
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

    // slider bar ontop
    DrawRectangle(0, 190, 280, 100, Fade(LIGHTGRAY, 0.5f));
    GuiSliderBar((Rectangle){100, 200, 120, 20}, "Acceleration X", "px/s^2", &accel_x, -1000, 1000);
    GuiSliderBar((Rectangle){100, 230, 120, 20}, "Acceleration Y", "px/s^2", &accel_y, -1000, 1000);
    GuiSliderBar((Rectangle){100, 260, 120, 20}, "Time Scale", NULL, &timescale, 0.5, 1.5);

    EndDrawing();
    calculate_delta_time();
}

int main() {
    init();
    emscripten_set_main_loop(update, TARGET_FPS, 0);
}

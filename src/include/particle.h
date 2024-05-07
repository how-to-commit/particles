#ifndef H_PARTICLE
#define H_PARTICLE

#include <point2f.h>
#include <stdbool.h>

typedef struct Particle {
    int ttl;  // in frames
    int mass; // radius and collision inertia

    // positional Vector2 -> all in units of pixels and seconds (e.g., accel = px/s^2)
    Point2f position_current;
    Point2f position_old;
    Point2f acceleration;
} Particle;

typedef struct PSystem {
    Particle particles[256];
    int next_free_slot;

    Point2f sys_accel;
} PSystem;

// particle functions
void particle_update_position(Particle *p, double dT);
void particle_constraint(Particle *p);
void particle_accelerate(Particle *p, Point2f accel);

// particle system solver
void psys_simulate(PSystem *system, double time_elapsed);
void handle_collision(Particle *p1, Particle *p2);
void collide_with_cursor(PSystem *system, Particle *cursor);

// system constructor
PSystem psys_init(Point2f sys_accel);
int psys_add(PSystem *system, Particle particle);

#endif

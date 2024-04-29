#ifndef H_PARTICLE
#define H_PARTICLE

#include <point2f.h>
#include <stdbool.h>

typedef struct Particle {
    // particle time to live in frames
    int ttl;

    // mass - used for calculating momentum, and (for now) as the radius of the particle
    int mass;

    // positional Vector2 -> all in units of pixels and seconds (e.g., accel = px/s^2)
    Point2f position;
    Point2f velocity;
    Point2f acceleration;
} Particle;

typedef struct PSystem {
    Particle particles[64];
    int next_free_slot;
} PSystem;

void particle_simulate(Particle *particle, double time_elapsed);

PSystem psys_init();
void psys_simulate(PSystem *system, double time_elapsed);
int psys_add(PSystem *system, Particle particle);

#endif

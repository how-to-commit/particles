#ifndef H_PARTICLE
#define H_PARTICLE

#include <point2f.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct Particle {
    int32_t ttl;

    float mass;

    Point2f position;
    Point2f velocity;
    Point2f acceleration;

    bool isDead;
} Particle;

void particle_init(Particle *ptr, int32_t ttl, Point2f *pos, Point2f *vel, Point2f *acc, bool isDead);
void particle_simulate(Particle *particle, double time_elapsed);

#endif

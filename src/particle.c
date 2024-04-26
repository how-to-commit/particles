#include <particle.h>
#include <point2f.h>

void particle_init(Particle *ptr, int32_t ttl, Point2f *pos, Point2f *vel, Point2f *acc, bool isDead) {
    ptr->ttl = ttl;
    ptr->position = *pos;
    ptr->velocity = *vel;
    ptr->acceleration = *acc;
    ptr->isDead = isDead;
}

void particle_simulate(Particle *particle, double time_elapsed) {
    // do not continue to simulate "dead" particles
    if (particle->isDead)
        return;

    // check and kill dead particles - but we still want them to finish this iteration
    // kill at zero, below zero we ignore
    if (particle->ttl > 0)
        particle->ttl--;
    if (particle->ttl == 0)
        particle->isDead = true;

    Point2f frac_vel = p2f_mul(&particle->velocity, time_elapsed);
    Point2f frac_acc = p2f_mul(&particle->acceleration, time_elapsed);

    p2f_add_inplace(&particle->position, &frac_vel);
    p2f_add_inplace(&particle->velocity, &frac_acc);
}

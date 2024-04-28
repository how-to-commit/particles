#include <raylib.h>

#include <particle.h>
#include <point2f.h>

void particle_init(Particle *ptr, int32_t ttl, uint32_t mass, Point2f *pos, Point2f *vel, Point2f *acc) {
    ptr->ttl = ttl;
    ptr->mass = mass;
    ptr->position = *pos;
    ptr->velocity = *vel;
    ptr->acceleration = *acc;
}

void particle_simulate(Particle *p, double time_elapsed) {
    // do not continue to simulate "dead" particles
    if (p->ttl == 0)
        return;

    // check and kill dead particles - but we still want them to finish this iteration
    if (p->ttl > 0)
        p->ttl--;

    // check if particle is on the boundary, if so, bounce them backward the other direction
    if (p->position.x >= (GetScreenWidth() - p->mass) || p->position.x <= p->mass)
        p->velocity.x *= -1.0f;
    if (p->position.y >= (GetScreenHeight() - p->mass) || p->position.y <= p->mass)
        p->velocity.y *= -1.0f;

    Point2f frac_vel = p2f_mul(&p->velocity, time_elapsed);
    Point2f frac_acc = p2f_mul(&p->acceleration, time_elapsed);

    p2f_add_inplace(&p->position, &frac_vel);
    p2f_add_inplace(&p->velocity, &frac_acc);
}

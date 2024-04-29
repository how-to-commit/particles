#include <stdlib.h>

#include <raylib.h>

#include <particle.h>
#include <point2f.h>

// void particle_init(Particle *ptr, int ttl, int mass, Point2f *pos, Point2f *vel, Point2f *acc) {
//     ptr->ttl = ttl;
//     ptr->mass = mass;
//     ptr->position = *pos;
//     ptr->velocity = *vel;
//     ptr->acceleration = *acc;
// }

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

PSystem psys_init() {
    PSystem sys = {.particles = {0}, .next_free_slot = 0};
    return sys;
}

void psys_simulate(PSystem *system, double time_elapsed) {
    size_t sysSize = sizeof(system->particles) / sizeof(Particle);
    for (size_t i = 0; i < sysSize; i++) {
        particle_simulate(&system->particles[i], time_elapsed);
    }
}

int psys_add(PSystem *system, Particle particle) {
    size_t sysSize = sizeof(system->particles) / sizeof(Particle);

    if (system->next_free_slot > 0) {
        system->particles[system->next_free_slot] = particle;

        if ((size_t)system->next_free_slot < sysSize - 2) // have one slot free
            system->next_free_slot++;
        else
            system->next_free_slot = -1;

        return 1;
    }

    // no known free slots, but we make a best attempt
    // e.g., particle may have died and thus we have a free slot
    for (size_t i = 0; i < sysSize; i++) {
        if (system->particles[i].ttl == 0) {
            system->particles[i] = particle;
            return 1;
        }
    }

    // we failed to find any slots
    return -1;
}

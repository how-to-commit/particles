#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <raylib.h>

#include <particle.h>
#include <point2f.h>

// --------------------------------------------------------------------------------------------------------------------
// particles
// --------------------------------------------------------------------------------------------------------------------

void particle_update_position(Particle *p, double dT) {
    // use Verlet integration to calculate movement

    // probably refactor the math
    // position_current = 2 * position_current - position_old + acceleration * dT^2
    const Point2f deltaPos = p2f_sub(p->position_current, p->position_old);
    p->position_old = p->position_current;
    p->position_current = p2f_add(p2f_add(p->position_current, deltaPos), p2f_fmul(p->acceleration, dT * dT));

    // reset accel
    p->acceleration = (Point2f){0, 0};
}

void particle_constraint(Particle *p) {
    const int max_height = GetScreenHeight();
    const int max_width = GetScreenWidth();

    const double ELASTICITY = 0.8;
    double dx = (p->position_current.x - p->position_old.x) * ELASTICITY;
    double dy = (p->position_current.y - p->position_old.y) * ELASTICITY;

    if (p->position_current.x > max_width - p->mass) {
        p->position_current.x = max_width - p->mass;
        p->position_old.x = p->position_current.x + dx;
    } else if (p->position_current.x < p->mass) {
        p->position_current.x = p->mass;
        p->position_old.x = p->position_current.x + dx;
    }

    if (p->position_current.y > max_height - p->mass) {
        p->position_current.y = max_height - p->mass;
        p->position_old.y = p->position_current.y + dy;
    } else if (p->position_current.y < p->mass) {
        p->position_current.y = p->mass;
        p->position_old.y = p->position_current.y + dy;
    }
}

void particle_accelerate(Particle *p, Point2f accel) {
    p->acceleration = p2f_add(p->acceleration, accel);
}

// --------------------------------------------------------------------------------------------------------------------
// system solver
// --------------------------------------------------------------------------------------------------------------------

void psys_simulate(PSystem *system, double time_elapsed) {
    size_t sysSize = sizeof(system->particles) / sizeof(Particle);

    for (size_t i = 0; i < sysSize; i++) {
        if (system->particles[i].ttl == 0)
            continue;

        particle_accelerate(&system->particles[i], system->sys_accel);
        particle_constraint(&system->particles[i]);

        for (size_t j = 0; j < sysSize; j++) {
            if (i != j && system->particles[j].ttl != 0)
                handle_collision(&system->particles[i], &system->particles[j]);
        }

        particle_update_position(&system->particles[i], time_elapsed);
    }
}

void handle_collision(Particle *p1, Particle *p2) {

    Point2f collision_vector = p2f_sub(p1->position_current, p2->position_current);
    double dist = p2f_dist(collision_vector);
    double mindist = (double)(p1->mass + p2->mass);

    if (dist < mindist) {
        Point2f collision_direction = p2f_get_unit_vector(collision_vector);
        double delta_pos = (mindist - dist) / 2;
        double p1_mass_ratio = (double)p1->mass / (double)(p1->mass + p2->mass);
        double p2_mass_ratio = (double)p2->mass / (double)(p1->mass + p2->mass);

        p1->position_current = p2f_add(p1->position_current, p2f_fmul(collision_direction, delta_pos * p2_mass_ratio));
        p2->position_current = p2f_sub(p2->position_current, p2f_fmul(collision_direction, delta_pos * p1_mass_ratio));
    }
}

void collide_with_cursor(PSystem *system, Particle *cursor) {
    size_t sysSize = sizeof(system->particles) / sizeof(Particle);

    for (size_t i = 0; i < sysSize; i++) {
        if (system->particles[i].ttl == 0)
            continue;

        handle_collision(cursor, &system->particles[i]);
    }
}

// --------------------------------------------------------------------------------------------------------------------
// particle system constructors
// --------------------------------------------------------------------------------------------------------------------

PSystem psys_init(Point2f sys_accel) {
    PSystem sys = {.particles = {0}, .next_free_slot = 0, .sys_accel = sys_accel};
    return sys;
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

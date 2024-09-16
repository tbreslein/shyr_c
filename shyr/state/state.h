#pragma once
#include "config/config.h"

typedef struct {
    double p[N_ALL][J_MAX][N_DIRS];
    double c[N_ALL][J_MAX][N_DIRS];
    double eigen_vals[N_ALL][J_MAX][N_DIRS];
    double flux[N_ALL][J_MAX][N_DIRS];
    double c_sound[N_ALL][N_DIRS];
    double adiabatic_index;
} State;

State init_state(const Config *const config) {
    return (State){.adiabatic_index = config->adiabatic_index};
}

void update_prim(State *u, const int dir) {
    const double gamma_minus_one = (u->adiabatic_index - 1.0);
    for (int i = 0; i < N_ALL; i++) {
        u->p[dir][J_DENS][i] = u->c[dir][J_DENS][i];
#if PHYSICS_TYPE == PHYSICS_EULER
        u->p[dir][J_XI][i] = u->c[dir][J_XI][i] / u->c[dir][J_DENS][i];
        u->p[dir][J_P][i] =
            gamma_minus_one *
            (u->c[dir][J_P][i] - 0.5 / u->c[dir][J_DENS][i] *
                                     u->c[dir][J_XI][i] * u->c[dir][J_XI][i]);
#endif
#if PHYSICS_TYPE == PHYSICS_EULERISO
#endif
    }
}

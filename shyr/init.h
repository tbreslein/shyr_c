#pragma once

#include "config.h"
#include "mesh.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>

void init_noh(
    double p[restrict J_MAX][N_ALL], [[maybe_unused]] double cs[restrict N_ALL])
{
    const int midpoint = N_ALL / 2;
    for (int i = 0; i < midpoint; i++) {
        p[J_DENS][i] = 1.0;
        p[J_XI][i] = 1.0;
#if PHYSICS_TYPE == PHYSICS_EULER
        p[J_P][i] = 1.0e-5;
#elif PHYSICS_TYPE == PHYSICS_EULERISO
        cs[i] = 1.0;
#endif
    }
    for (int i = midpoint; i < N_ALL; i++) {
        p[J_DENS][i] = 1.0;
        p[J_XI][i] = -1.0;
#if PHYSICS_TYPE == PHYSICS_EULER
        p[J_P][i] = 1.0e-5;
#elif PHYSICS_TYPE == PHYSICS_EULERISO
        cs[i] = 1.0;
#endif
    }
}

void apply_initial_conditions(
    const InitialConditions init_cond, State *u,
    [[maybe_unused]] const Mesh *mesh)
{
    switch (init_cond) {
        case InitNoh:
            init_noh(u->p[CENT], u->c_sound[CENT]);
            break;
        default:
            fprintf(
                stderr,
                "case for apply_initial_conditions not covered; init_cond = "
                "%d\n",
                init_cond);
            exit(1);
    }

    for (int j = 0; j < J_MAX; j++)
        for (int i = 0; i < N_ALL; i++) {
            u->p[WEST][j][i] = u->p[CENT][j][i];
            u->p[EAST][j][i] = u->p[CENT][j][i];
        }
}

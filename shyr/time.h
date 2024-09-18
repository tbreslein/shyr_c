#pragma once

#include "config.h"
#include "mesh.h"
#include "rhs.h"
#include "state.h"
#include <stdio.h>

typedef struct {
    double err_new;
    double err_old;
    int n_asc;
    double relative_tolerance;
    double absolute_tolerance;
    double timestep_friction;
    bool solution_accepted;
    double ucons_low[J_MAX][N_ALL];
} Asc;

typedef struct {
    RkfMethod tag;
    double mat[5][5];
} Butchertableau;

int rkf_rank(RkfMethod tag)
{
    switch (tag) {
        case RK1:
            return 1;
        case RKF4:
            return 5;
        case SSPRK3:
            return 3;
        case SSPRK5:
            return 6;
        default:
            fprintf(
                stderr, "Non-exhaustive switch-case at %s, line %d", __FILE__,
                __LINE__);
    }
}

typedef struct {
    Butchertableau bt;
    double k_bundle[5][J_MAX][N_ALL];
    double utilde[N_DIRS][J_MAX][N_ALL];
    Asc asc;
} TimeSolver;

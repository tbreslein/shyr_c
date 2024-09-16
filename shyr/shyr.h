#pragma once

#include "config.h"
#include "init.h"
#include "mesh.h"
#include "state.h"
#include <stdio.h>

int shyr_run(const Config config)
{
    const Mesh mesh = init_mesh(&config);
    State u = init_state(&config);
    apply_initial_conditions(InitNoh, &u, &mesh);
    for (int i = 0; i < N_ALL; i++)
        printf("xi_cent[%d] = %f\n", i, mesh.xi_cent[i]);
    for (int i = 0; i < N_ALL; i++)
        printf("xi_west[%d] = %f\n", i, mesh.xi_west[i]);
    for (int i = 0; i < N_ALL; i++)
        printf("xi_east[%d] = %f\n", i, mesh.xi_east[i]);

    for (int j = 0; j < J_MAX; j++)
        for (int i = 0; i < N_ALL; i++)
            printf("u.p[CENT][%d][%d] = %f\n", j, i, u.p[CENT][j][i]);
    return 0;
}

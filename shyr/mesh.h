#pragma once

#include "config.h"

typedef struct {
    double xi_cent[N_ALL];
    double xi_west[N_ALL];
    double xi_east[N_ALL];
    double d_area_xi_deta_dphi_east[N_ALL];
} Mesh;

Mesh init_mesh(const Config *const config)
{
    Mesh mesh = {};

    const double dxi = (config->xi_out - config->xi_in) / (double)N_COMP;
    for (int i = 0; i < N_ALL; i++) {
        mesh.xi_west[i] = config->xi_in + dxi * (double)(i - IXI_IN);
        mesh.xi_cent[i] = mesh.xi_west[i] + 0.5 * dxi;
        mesh.xi_east[i] = mesh.xi_west[i] + dxi;
    }

    return mesh;
}

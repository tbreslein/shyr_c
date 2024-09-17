#pragma once

#include "config.h"
#include "mesh.h"
#include "state.h"
#include <math.h>
#include <stdio.h>

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

typedef struct {
    double aplus[N_ALL];
    double aminus[N_ALL];
    double helper_b[N_ALL];
    double helper_c[N_ALL];
} NumfluxHelpers;

typedef struct {
    BoundaryCond boundary_west;
    BoundaryCond boundary_east;
    NumfluxHelpers helpers;
    /*double sum_source[J_MAX][N_ALL];*/
    double numflux[J_MAX][N_ALL];
    double rhs_full[J_MAX][N_ALL];
} Rhs;

Rhs init_rhs(const Config *const config)
{
    return (Rhs){
        .boundary_west = config->boundary_west,
        .boundary_east = config->boundary_east};
}

void reconstruct(State *const u, const Mesh *const mesh)
{
    // NOTE: this just uses the vanleer limiter, but maybe I could implement
    // more
    // TODO: needs to be more generic for non-cartesian meshes
    const double inv_dxi =
        1.0 / (mesh->xi_east[N_ALL - 1] - mesh->xi_west[N_ALL - 1]);
    const double dist_west =
        mesh->xi_west[N_ALL - 1] - mesh->xi_cent[N_ALL - 1];
    const double dist_east =
        mesh->xi_east[N_ALL - 1] - mesh->xi_cent[N_ALL - 1];
    for (int j = 0; j < J_MAX; j++) {
        for (int i = 1; i < N_ALL - 1; i++) {
            double a = u->c[CENT][j][i] - u->c[CENT][j][i - 1];
            double b = u->c[CENT][j][i + 1] - u->c[CENT][j][i];
            double slope = inv_dxi * (a * fabs(b) + b * fabs(a));
            u->c[WEST][j][i] = u->c[CENT][j][i] + slope * dist_west;
            u->c[EAST][j][i] = u->c[CENT][j][i] + slope * dist_east;
        }
    }
}

void update_numflux_helpers(
    double aplus[restrict N_ALL], double aminus[restrict N_ALL],
    double helper_b[restrict N_ALL], double helper_c[restrict N_ALL],
    const State *const u)
{
    for (int i = IXI_IN - 1; i < IXI_OUT + 1; i++) {
        aplus[i] =
            MAX(0.0, MAX(u->p[WEST][J_XI][i + 1] + u->c_sound[WEST][i + 1],
                         u->p[EAST][J_XI][i] + u->c_sound[EAST][i]));
        aminus[i] =
            MIN(0.0, MIN(u->p[WEST][J_XI][i + 1] - u->c_sound[WEST][i + 1],
                         u->p[EAST][J_XI][i] - u->c_sound[EAST][i]));
        helper_b[i] = 1.0 / (aplus[i] - aminus[i]);
        helper_c[i] = aplus[i] * aminus[i];
    }
}

void update_numflux(
    double numflux[restrict J_MAX][N_ALL], NumfluxHelpers *const helpers,
    State *const u, const Mesh *const mesh)
{
    reconstruct(u, mesh);
    update_primitive_vars(u->p[WEST], u->c[WEST], u->adiabatic_index);
    update_primitive_vars(u->p[EAST], u->c[EAST], u->adiabatic_index);
    update_c_sound(u->c_sound[WEST], u->p[WEST], u->adiabatic_index);
    update_c_sound(u->c_sound[EAST], u->p[EAST], u->adiabatic_index);
    update_physical_flux(
        u->flux[WEST], u->p[WEST], u->c[WEST], u->c_sound[WEST]);
    update_physical_flux(
        u->flux[EAST], u->p[EAST], u->c[EAST], u->c_sound[EAST]);

    update_numflux_helpers(
        helpers->aplus, helpers->aminus, helpers->helper_b, helpers->helper_c,
        u);

    for (int j = 0; j < J_MAX; j++)
        for (int i = IXI_IN - 1; i < IXI_OUT + 1; i++)
            numflux[j][i] = helpers->helper_b[i] *
                            (helpers->aplus[i] * u->flux[EAST][j][i] -
                             helpers->aminus[i] * u->flux[WEST][j][i + 1] +
                             helpers->helper_c[i] *
                                 (u->c[WEST][j][i + 1] - u->c[EAST][j][i]));
}

void update_boundaries(
    const BoundaryCond boundary_west, const BoundaryCond boundary_east,
    double p[restrict J_MAX][N_ALL])
{
    switch (boundary_west) {
        case BoundaryNoGradients:
            for (int j = 0; j < J_MAX; j++)
                for (int i = IXI_IN - 1; i >= 0; i--)
                    p[j][i] = p[j][IXI_IN];
            break;
        default:
            fprintf(
                stderr, "non-exhaustive case in %s, line %d", __FILE__,
                __LINE__);
    }
    switch (boundary_east) {
        case BoundaryNoGradients:
            for (int j = 0; j < J_MAX; j++)
                for (int i = IXI_OUT + 1; i < N_ALL; i++)
                    p[j][i] = p[j][IXI_OUT];
            break;
        default:
            fprintf(
                stderr, "non-exhaustive case in %s, line %d", __FILE__,
                __LINE__);
    }
}

void update_rhs(Rhs *const rhs, State *const u, const Mesh *const mesh)
{
    update_boundaries(rhs->boundary_west, rhs->boundary_east, u->p[CENT]);
    update_conservative_vars(u->c[CENT], u->p[CENT], u->adiabatic_index);
    update_numflux(rhs->numflux, &rhs->helpers, u, mesh);

    // TODO: needs generic 1/volume for non-cartesian grids
    const double inv_dxi =
        1.0 / (mesh->xi_east[N_ALL - 1] - mesh->xi_west[N_ALL - 1]);
    for (int j = 0; j < J_MAX; j++)
        for (int i = IXI_IN; i < IXI_OUT + 1; i++)
            rhs->rhs_full[j][i] =
                inv_dxi * (rhs->numflux[j][i] - rhs->numflux[j][i - 1]);
}

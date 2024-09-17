#pragma once

#include "config.h"
#include <math.h>

typedef struct {
    double p[N_DIRS][J_MAX][N_ALL];
    double c[N_DIRS][J_MAX][N_ALL];
    double eigen_vals[N_DIRS][J_MAX][N_ALL];
    double flux[N_DIRS][J_MAX][N_ALL];
    double c_sound[N_DIRS][N_ALL];
    double adiabatic_index;
} State;

State init_state(const Config *const config)
{
    return (State){.adiabatic_index = config->adiabatic_index};
}

void update_primitive_vars(
    double p[restrict J_MAX][N_ALL], const double c[restrict J_MAX][N_ALL],
    [[maybe_unused]] const double gamma)
{
#if PHYSICS_TYPE == PHYSICS_EULER
    const double gamma_minus_one = (gamma - 1.0);
#endif

    for (int i = 0; i < N_ALL; i++) {
        p[J_DENS][i] = c[J_DENS][i];
        p[J_XI][i] = c[J_XI][i] / c[J_DENS][i];

#if PHYSICS_TYPE == PHYSICS_EULER
        p[J_P][i] = gamma_minus_one *
                    (c[J_P][i] - 0.5 / c[J_DENS][i] * c[J_XI][i] * c[J_XI][i]);
#endif
    }
}

void update_conservative_vars(
    double c[restrict J_MAX][N_ALL], const double p[restrict J_MAX][N_ALL],
    [[maybe_unused]] const double gamma)
{
#if PHYSICS_TYPE == PHYSICS_EULER
    const double gamma_minus_one = (gamma - 1.0);
#endif

    for (int i = 0; i < N_ALL; i++) {
        c[J_DENS][i] = p[J_DENS][i];
        c[J_XI][i] = p[J_XI][i] * p[J_DENS][i];

#if PHYSICS_TYPE == PHYSICS_EULER
        c[J_P][i] = p[J_P][i] / gamma_minus_one +
                    0.5 * p[J_DENS][i] * p[J_XI][i] * p[J_XI][i];
#endif
    }
}

void update_physical_flux(
    double flux[restrict J_MAX][N_ALL], const double p[restrict J_MAX][N_ALL],
    const double c[restrict J_MAX][N_ALL],
    [[maybe_unused]] const double cs[restrict N_ALL])
{
    for (int i = 0; i < N_ALL; i++) {
        flux[J_DENS][i] = c[J_XI][i];
#if PHYSICS_TYPE == PHYSICS_EULER
        flux[J_XI][i] = p[J_XI][i] * c[J_XI][i] + p[J_P][i];
        flux[J_P][i] = (c[J_XI][i] + p[J_XI][i]) * p[J_XI][i];
#elif PHYSICS_TYPE == PHYSICS_EULERISO
        flux[J_XI][i] =
            p[J_DENS][i] * (p[J_XI][i] * p[J_XI][i] + cs[i] * cs[i]);
#endif
    }
}

void update_c_sound(
    [[maybe_unused]] double cs[restrict N_ALL],
    [[maybe_unused]] const double p[restrict J_MAX][N_ALL],
    [[maybe_unused]] const double gamma)
{
#if PHYSICS_TYPE == PHYSICS_EULERISO
    return;
#elif PHYSICS_TYPE == PHYSICS_EULER
    for (int i = 0; i < N_ALL; i++)
        cs[i] = sqrt(gamma * p[J_P][i] / p[J_DENS][i]);
#endif
}

/*void update_eigen_vals(*/
/*    double eigen_vals[restrict J_MAX][N_ALL], const double cs[restrict
 * N_ALL],*/
/*    const double p[restrict J_MAX][N_ALL])*/
/*{*/
/*    for (int i = 0; i < N_ALL; i++) {*/
/*        eigen_vals[0][i] = p[J_XI][i] - cs[i];*/
/*        eigen_vals[J_MAX - 1][i] = p[J_XI][i] + cs[i];*/
/*    }*/
/*    for (int j = 1; j < J_MAX - 1; j++)*/
/*        for (int i = 0; i < N_ALL; i++)*/
/*            eigen_vals[j][i] = p[J_XI][i];*/
/*}*/

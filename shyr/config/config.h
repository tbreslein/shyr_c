#pragma once

#ifndef N_COMP
#define N_COMP 100
#endif

#ifndef N_GC
#define N_GC 2
#endif

#ifndef N_ALL
#define N_ALL (N_GC * 2 + N_COMP)
#endif

#define PHYSICS_EULERISO 1
#define PHYSICS_EULER 2

#ifndef PHYSICS_TYPE
#define PHYSICS_TYPE PHYSICS_EULER
#endif

#if PHYSICS_TYPE == PHYSICS_EULERISO
#define J_MAX 2
#define IS_ADIABATIC 0
#define J_DENS 0
#define J_XI 1
#define J_P 0
#endif

#if PHYSICS_TYPE == PHYSICS_EULER
#define J_MAX 3
#define IS_ADIABATIC 1
#define J_DENS 0
#define J_XI 1
#define J_P 2
#endif

#define WEST 0
#define CENT 1
#define EAST 2
#define N_DIRS 3

typedef struct Config {
    double xi_in;
    double xi_out;
    double adiabatic_index;
} Config;

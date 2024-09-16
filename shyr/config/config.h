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
#define J_MAX 3
#endif

#if PHYSICS_TYPE == PHYSICS_EULER
#define J_MAX 4
#endif

typedef struct Config {
    double xi_in;
    double xi_out;
    double adiabatic_index;
} Config;

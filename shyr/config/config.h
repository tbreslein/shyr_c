#ifndef N_COMP
#error "N_COMP needs to be defined before including shyr/config.h"
#endif

#ifndef N_GC
#define N_GC 2
#endif

#ifndef N_ALL
#define N_ALL (N_GC * 2 + N_COMP)
#endif

#ifndef PHYSICS_TYPE
#error "PHYSICS_TYPE needs to be defined before including shyr/config.h"
#endif

#define PHYSICS_EULERISO 1
#define PHYSICS_EULER 2

#if PHYSICS_TYPE == PHYSICS_EULERISO
#define J_MAX 3
#endif

#if PHYSICS_TYPE == PHYSICS_EULER
#define J_MAX 4
#endif

typedef enum MeshType { MeshCartesian } MeshType;

typedef struct PhysicsConfig {
    double adiabatic_index;
} PhysicsConfig;

typedef struct Config {
    MeshType mesh_type;
    PhysicsConfig physics_config;
} Config;

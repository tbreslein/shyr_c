#define N_COMP 10
#define PHYSICS_TYPE 1

#include <shyr.h>

int main(void)
{
    return shyr_run((Config){
        .adiabatic_index = 1.0,
        .xi_in = 1.0,
        .xi_out = 10.0,
        .init_conditions = InitNoh});
}

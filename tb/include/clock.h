#ifndef CLOCK_H
#define CLOCK_H

#include "Vint_dct8.h"
#include <cstdint>

extern uint64_t main_time;

// callback for Verilator to retrieve simulation time
double sc_time_stamp();

// advance the simulation by one clock cycle
void toggle_clock(Vint_dct8 *top);

#endif

#include "agent.h"
#include "clock.h"
#include <iostream>

// global time
uint64_t cycle_time = 0;

double sc_time_stamp() { return cycle_time; } // this design is time-invariant

void toggle_clock(Vint_dct8 *top)
{
  // low phase
  top->clk = 0;
  top->eval();
  // high phase
  top->clk = 1;
  top->eval();
  cycle_time++;
}

Agent::Agent(Vint_dct8 *dut) : top(dut) {}

void Agent::drive(const int8_t input[N])
{
  top->valid_in = 1;
  for (int i = 0; i < N; i++)
  {
    top->x[i] = input[i];
  }
  toggle_clock(top);
  top->valid_in = 0;
}

void Agent::monitor(int32_t output[N])
{
  const int MAX_WAIT = 20;
  int wait_cycles = 0;
  while (!top->valid_out && wait_cycles < MAX_WAIT)
  {
    toggle_clock(top);
    wait_cycles++;
  }
  if (!top->valid_out)
  {
    std::cerr << "Error: valid_out not asserted within " << MAX_WAIT
              << " cycles.\n";
    for (int i = 0; i < N; i++)
    {
      output[i] = 0;
    }
    return;
  }
  for (int i = 0; i < N; i++)
  {
    if (top->y[i] & (1 << 19))
    {
      output[i] = top->y[i] | ~((1 << 20) - 1); // sign extend
    }
    else
    {
      output[i] = top->y[i];
    }
  }
}

void Agent::reset()
{
  top->rst = 1;
  top->valid_in = 0;
  toggle_clock(top);
  top->rst = 0;
}

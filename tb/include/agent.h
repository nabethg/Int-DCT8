#ifndef AGENT_H
#define AGENT_H

#include "Vint_dct8.h"
#include "reference.h" // for N
#include <cstdint>

// encapsulates DUT interactions
class Agent
{
public:
  explicit Agent(Vint_dct8 *dut);

  void drive(const int8_t input[N]);

  void monitor(int32_t output[N]);

  void reset(); // single cycle reset

private:
  Vint_dct8 *top;
};

#endif

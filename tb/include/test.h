#ifndef TEST_H
#define TEST_H

#include "agent.h"
#include "reference.h" // for N
#include "scoreboard.h"
#include <cstdint>

typedef void (*ref_func)(const int8_t input[N], int32_t output[N]);

// use an Agent to run test cases against a reference model.
class Test {
  public:
    explicit Test(Agent *agent, ref_func ref_model);

    void run_custom_tests();
    void run_random_tests(int num_random);
    void run_all_tests(int num_random);
    const Scoreboard &getScoreboard() const;
    void resetScoreboard();

  private:
    Agent *agent;
    Scoreboard scoreboard;
    ref_func ref_model;

    void run_vec_test(const int8_t test_case[N]);
    void generate_rand_vec(int8_t test_vec[N]);
};

#endif

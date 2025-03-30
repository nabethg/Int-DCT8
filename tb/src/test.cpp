#include "test.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>

inline static bool compare_arrays(const int32_t a[N], const int32_t b[N],
                                  int eps) {
    for (int i = 0; i < N; i++) {
        if (std::abs(a[i] - b[i]) > eps)
            return false;
    }
    return true;
}

inline static void print_arr_int(const int8_t arr[N]) {
    for (int i = 0; i < N; i++) {
        std::cout << static_cast<int>(arr[i]) << " ";
    }
}

inline static void print_arr_int(const int32_t arr[N]) {
    for (int i = 0; i < N; i++) {
        std::cout << arr[i] << " ";
    }
}

static const int8_t custom_tests[][N] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {5, 5, 5, 5, 5, 5, 5, 5},
    {1, -1, 1, -1, 1, -1, 1, -1},
    {0, 1, 2, 3, 4, 5, 6, 7},
    {-128, -128, -128, -128, -128, -128, -128, -128},
    {-128, 127, -128, 127, -128, 127, -128, 127}};

static constexpr int num_custom_tests =
    sizeof(custom_tests) / sizeof(custom_tests[0]);

Test::Test(Agent *agent, ref_func ref_model)
    : agent(agent), ref_model(ref_model) {}

void Test::run_vec_test(const int8_t test_case[N]) {
    int32_t hw_output[N] = {0};
    agent->drive(test_case);
    agent->monitor(hw_output);

    int32_t ref_output[N] = {0};
    ref_model(test_case, ref_output);

    constexpr int TOL = 1;
    bool passTest = compare_arrays(hw_output, ref_output, TOL);

    if (passTest) {
        scoreboard.pass();
    } else {
        scoreboard.fail();
        std::cout << "Test FAILED for test case: ";
        print_arr_int(test_case);
        std::cout << "\nHardware Output: ";
        print_arr_int(hw_output);
        std::cout << "\nReference Output: ";
        print_arr_int(ref_output);
        std::cout << "\n\n";
    }
}

void Test::generate_rand_vec(int8_t test_vec[N]) {
    for (int i = 0; i < N; i++) {
        // random values in the range [-128, 127]
        test_vec[i] = static_cast<int8_t>((std::rand() % 256) - 128);
    }
}

void Test::run_custom_tests() {
    for (int i = 0; i < num_custom_tests; i++) {
        run_vec_test(custom_tests[i]);
    }
}

void Test::run_random_tests(int num_random) {
    int8_t rand_vec[N];
    for (int i = 0; i < num_random; i++) {
        generate_rand_vec(rand_vec);
        run_vec_test(rand_vec);
    }
}

void Test::run_all_tests(int num_random) {
    run_custom_tests();
    run_random_tests(num_random);
}

const Scoreboard &Test::getScoreboard() const { return scoreboard; }

void Test::resetScoreboard() { scoreboard.reset(); }

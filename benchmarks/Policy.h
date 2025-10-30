

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_POLICY_H
#define EFFICIENT_MULTI_ARMED_BANDITS_POLICY_H
template <typename Bandit>
static void benchmark_policy(benchmark::State &state)
{
    Bandit b(state.range(0),state.range(1)*0.1);
    int microseconds = 0;
    std::chrono::duration<double, std::micro> sleep_duration {
        static_cast<double> (microseconds)
    };
    //auto k = state.range(0);
    for (auto _ : state)
    {
        int rounds = 1000;
        for (int i = 0; i < rounds; i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            int choice = b.choose();
            b.give_reward(choice, (i % 3)/3.0);
            auto end = std::chrono::high_resolution_clock::now();

            auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end-start);
            state.SetIterationTime(elapsed_time.count());
        }
    }
}

template <typename Bandit>
static void benchmark_policy_update(benchmark::State &state)
{
    Bandit b(state.range(0),state.range(1)*0.1);
    int microseconds = 0;
    std::chrono::duration<double, std::micro> sleep_duration {
            static_cast<double> (microseconds)
    };
    //auto k = state.range(0);
    for (auto _ : state)
    {
        int rounds = 1000;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            auto start = std::chrono::high_resolution_clock::now();
            b.give_reward(choice, (i % 3)/3.0);
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end- start);
            state.SetIterationTime(elapsed_time.count());
        }
    }
}

template <typename Bandit>
static void benchmark_policy_sample(benchmark::State &state)
{
    int microseconds = 0;
    std::chrono::duration<double, std::micro> sleep_duration {
            static_cast<double> (microseconds)
    };
    Bandit b(state.range(0),state.range(1)*0.1);
    //auto k = state.range(0);
    for (auto _ : state)
    {
        int rounds = 1000;
        for (int i = 0; i < rounds; i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            int choice = b.choose();
            auto end = std::chrono::high_resolution_clock::now();
            b.give_reward(choice, (i % 3)/3.0);
            auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end- start);
            state.SetIterationTime(elapsed_time.count());
        }
    }
}

template <typename Bandit>
static void benchmark_m_policy(benchmark::State &state)
{
    int m = state.range(1);
    Bandit b(m,state.range(0), 1);
    std::vector<double> reward(m, 0);

    int microseconds = 0;
    std::chrono::duration<double, std::micro> sleep_duration {
            static_cast<double> (microseconds)
    };
    //auto k = state.range(0);
    for (auto _ : state)
    {
        int rounds = 1000;
        for (int i = 0; i < rounds; i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto choices = b.choose(m);
            auto pause = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < m; j++) {
                reward[j] = ((i+choices[j])%3)/3.0;
            }
            auto pause1 = std::chrono::high_resolution_clock::now();
            b.give_reward(choices, reward);
            auto end = std::chrono::high_resolution_clock::now();

            auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end-start-(pause1-pause));
            state.SetIterationTime(elapsed_time.count());
        }
    }
}

template <typename Bandit>
static void benchmark_m_policy_update(benchmark::State &state)
{
    int m = state.range(1);
    Bandit b(m,state.range(0), 1);
    std::vector<double> reward(m, 0);

    int microseconds = 0;
    std::chrono::duration<double, std::micro> sleep_duration {
            static_cast<double> (microseconds)
    };
    //auto k = state.range(0);
    for (auto _ : state)
    {
        int rounds = 1000;
        for (int i = 0; i < rounds; i++)
        {
            auto choices = b.choose(m);
            for (int j = 0; j < m; j++) {
                reward[j] = ((i+choices[j])%3)/3.0;
            }
            auto start = std::chrono::high_resolution_clock::now();
            b.give_reward(choices, reward);
            auto end = std::chrono::high_resolution_clock::now();

            auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end-start);
            state.SetIterationTime(elapsed_time.count());
        }
    }
}
template <typename Bandit>
static void benchmark_m_policy_sample(benchmark::State &state)
{
    int m = state.range(1);
    Bandit b(m,state.range(0), 1);
    std::vector<double> reward(m, 0);

    int microseconds = 0;
    std::chrono::duration<double, std::micro> sleep_duration {
            static_cast<double> (microseconds)
    };
    //auto k = state.range(0);
    for (auto _ : state)
    {
        int rounds = 1000;
        for (int i = 0; i < rounds; i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto choices = b.choose(m);
            auto end = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < m; j++) {
                reward[j] = ((i+choices[j])%3)/3.0;
            }
            b.give_reward(choices, reward);

            auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end-start);
            state.SetIterationTime(elapsed_time.count());
        }
    }
}


#endif //EFFICIENT_MULTI_ARMED_BANDITS_POLICY_H



static void benchmark_exp31(benchmark::State &state)
{
    auto k = state.range(0);
    Exp31 b(k);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(choice, i % 2);
        }
    }
}

static void benchmark_exp31_sample(benchmark::State &state)
{
    auto k = state.range(0);
    Exp31 b(k);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
        }
    }
}
static void benchmark_exp31_update(benchmark::State &state)
{
    auto k = state.range(0);
    Exp31 b(k);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(i % k, i % 2);
        }
    }
}



static void benchmark_exp31_optimized(benchmark::State &state)
{
    auto k = state.range(0);
    Exp31_optimized b(k);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(choice, i % 2);
        }
    }
}

static void benchmark_exp31_optimized_sample(benchmark::State &state)
{
    auto k = state.range(0);
    Exp31_optimized b(k);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
        }
    }
}
static void benchmark_exp31_optimized_update(benchmark::State &state)
{
    auto k = state.range(0);
    Exp31_optimized b(k);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(i % k, i % 2);
        }
    }
}
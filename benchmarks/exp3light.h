
static void benchmark_exp3_light(benchmark::State &state)
{
    auto k = state.range(0);
    Exp3Light b(k, 0.1);
    for (auto _ : state)
    {

        int rounds = 10000;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(choice, (i % 3)/3.0);
        }
    }
}

static void benchmark_exp3_light_sample(benchmark::State &state)
{
    auto k = state.range(0);
    Exp3Light b(k, 0.1);
    for (auto _ : state)
    {
        int rounds = 10000;
        for (int i = 0; i < rounds; i++)
        {
            benchmark::DoNotOptimize(b.choose());
        }
    }
}
static void benchmark_exp3_light_update(benchmark::State &state)
{
    auto k = state.range(0);
    Exp3Light b(k, 0.1);
    for (auto _ : state)
    {
        int rounds = 10000;
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(i%k, (i % 3)/3.0);
        }
    }
}


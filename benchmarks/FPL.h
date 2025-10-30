


static void benchmark_fpl(benchmark::State &state)
{
    auto k = state.range(0);
    FPL b(k, 10);
    for (auto _ : state)
    {
        int rounds = 10000;
        for (int i = 0; i < rounds; i++)
        {
            b.choose();
            b.give_reward(0, 0);
        }
    }
}

static void benchmark_fpl_sample(benchmark::State &state)
{
    auto k = state.range(0);
    FPL b(k, 10);
    for (auto _ : state)
    {
        int rounds = 10000;
        for (int i = 0; i < rounds; i++)
        {
            benchmark::DoNotOptimize(b.choose());
        }
    }
}
static void benchmark_fpl_update(benchmark::State &state)
{
    auto k = state.range(0);
    FPL b(k, 10);
    for (auto _ : state)
    {
        int rounds = 10000;
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(0, 0);
        }
    }
}


static void benchmark_fpl_hashing(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    FPL_hash b(k, 10, rounds);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(0, 0);
        }
    }
}

static void benchmark_fpl_hashing_sample(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    FPL_hash b(k, 10, rounds);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
        }
    }
}
static void benchmark_fpl_hashing_update(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    FPL_hash b(k, 10, rounds);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(0, 0);
        }
    }
}
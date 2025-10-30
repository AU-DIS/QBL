
static void benchmark_tsallis_ltu(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_LTU b(k);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(choice, d[choice][i]);
        }
    }
}

static void benchmark_tsallis_ltu_update(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_LTU b(k);
    int choice = b.choose();
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(choice, d[choice][i]);
        }
    }
}
static void benchmark_tsallis_ltu_sample(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_LTU b(k);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            // b.give_reward(choice, d[choice][i]);
        }
    }
}
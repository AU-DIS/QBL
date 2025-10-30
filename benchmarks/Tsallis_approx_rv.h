
static void benchmark_tsallis_approxrv(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_approx_rv b(k);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(choice, d[choice][i]);
        }
    }
}

static void benchmark_tsallis_approxrv_update(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_approx_rv b(k);
    int choice = b.choose();
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(choice, d[choice][i]);
        }
    }
}
static void benchmark_tsallis_approxrv_sample(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_approx_rv b(k);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            // b.give_reward(choice, d[choice][i]);
        }
    }
}

static void benchmark_tsallis_heap(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_Heap b(k);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(choice, d[choice][i]);
        }
    }
}

static void benchmark_tsallis_heap_update(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_Heap b(k);
    int choice = b.choose();
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(choice, d[choice][i]);
        }
    }
}
static void benchmark_tsallis_heap_sample(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_Heap b(k);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            // b.give_reward(choice, d[choice][i]);
        }
    }
}
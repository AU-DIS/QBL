
static void benchmark_tsallis_rv(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    auto g = StochasticallyConstrainedDataset(k, rounds, 1, 0.9);
    auto d = g.generate();
    Tsallis_RV b(k);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(choice, d[choice][i]);
        }
    }
}
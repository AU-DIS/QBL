


static void benchmark_fpl_bucket(benchmark::State &state)
{
    auto k = state.range(0);
    FPL_buckets b(k, 0.1);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(0, 0);
        }
    }
}

static void benchmark_fpl_bucket_sample(benchmark::State &state)
{
    auto k = state.range(0);
    FPL_buckets b(k, 0.1);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
        }
    }
}
static void benchmark_fpl_bucket_update(benchmark::State &state)
{
    auto k = state.range(0);
    FPL_buckets b(k, 0.1);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(0, 0);
        }
    }
}
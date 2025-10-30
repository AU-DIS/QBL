

static void benchmark_fpl_toplog(benchmark::State &state)
{
    auto k = state.range(0);
    FPL_toplog b(k, 10);
    for (auto _ : state)
    {
        int rounds = 1000000;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(choice, 1);
        }
    }
}

static void benchmark_fpl_toplog_sample(benchmark::State &state)
{
    auto k = state.range(0);
    FPL_toplog b(k, 10);
    for (auto _ : state)
    {
        int rounds = 1000000;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
        }
    }
}
static void benchmark_fpl_toplog_update(benchmark::State &state)
{
    auto k = state.range(0);
    FPL_toplog b(k, 10);
    b.choose();
    for (auto _ : state)
    {
        int rounds = 1000000;
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(1, 1);
        }
    }
}
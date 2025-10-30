
static void benchmark_exp3_fid(benchmark::State &state)
{
    auto k = state.range(0);
    Exp3_fid b(k, 0.1, 512);
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

static void benchmark_exp3_fid_sample(benchmark::State &state)
{
    auto k = state.range(0);
    Exp3_fid b(k, 0.1, 512);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
        }
    }
}

static void benchmark_exp3_fid_update(benchmark::State &state)
{
    auto k = state.range(0);
    Exp3_fid b(k, 0.1, 512);
    for (auto _ : state)
    {
        int rounds = 100;
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(0, 0);
        }
    }
}

static void benchmark_exp3_lad(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    double gap = 3.2;
    double delta = 0.9;
    auto dataset = StochasticallyConstrainedDataset(k, rounds, gap, delta);
    auto d = dataset.generate();
    Exp3_lad b(k, 0.1, 0.8);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
            b.give_reward(choice, d[choice][i]);
        }
    }
}
static void benchmark_exp3_lad_sample(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    double gap = 3.2;
    double delta = 0.9;
    auto dataset = StochasticallyConstrainedDataset(k, rounds, gap, delta);
    auto d = dataset.generate();
    // auto d = mod_2(k, rounds, gap);
    Exp3_lad b(k, 0.1, 1);
    b.give_reward(0, 1);
    b.give_reward(1, 1);
    b.give_reward(2, 1);
    b.give_reward(3, 1);
    b.give_reward(4, 1);
    b.give_reward(5, 1);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            int choice = b.choose();
        }
    }
}

static void benchmark_exp3_lad_update(benchmark::State &state)
{
    auto k = state.range(0);
    int rounds = 100;
    double gap = 3.2;
    double delta = 0.9;
    auto dataset = StochasticallyConstrainedDataset(k, rounds, gap, delta);
    auto d = dataset.generate();
    Exp3_lad b(k, 0.1, 1);
    int choice = b.choose();
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(i % k, d[i % k][i]);
        }
    }
}
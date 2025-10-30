

static void benchmark_qbl(benchmark::State &state)
{
    auto k = state.range(0);
    QBL b(k, 0.1);
    for (auto _ : state)
    {
        int rounds = 10000;
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(b.choose(), (i % 3)/3.0);
        }
    }
}

static void benchmark_qbl_sample(benchmark::State &state)
{
    auto k = state.range(0);
    QBL b(k, 0.1);
    for (auto _ : state)
    {
        int rounds = 10000;
        for (int i = 0; i < rounds; i++)
        {
            benchmark::DoNotOptimize(b.choose());
        }
    }
}
static void benchmark_qbl_update(benchmark::State &state)
{
    auto k = state.range(0);
    QBL b(k, 0.1);
    for (auto _ : state)
    {
        int rounds = 10000;
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(i%k, (i % 3)/3.0);
        }
    }
}

static void benchmark_qbltopk(benchmark::State &state)
{
    int k = state.range(0);
    int m = (int)k / 10;
    int rounds = 100;

    std::vector<double> reward(k, 1);
    std::vector<int> index(k, 1);
    QBL b(k, 10);

    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            auto choices = b.choose(m);
            b.give_reward(index, reward);
        }
    }
}

static void benchmark_qbltopk_sample(benchmark::State &state)
{
    int k = state.range(0);
    int m = (int)k / 10;
    int rounds = 100;
    std::vector<double> reward(k, 1);
    std::vector<int> index(k, 1);
    QBL b(k, 10);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            auto choices = b.choose(m);
        }
    }
}

static void benchmark_qbltopk_update(benchmark::State &state)
{
    int k = state.range(0);
    int m = (int)k / 10;
    int rounds = 100;
    std::vector<double> reward(k, 0);
    std::vector<int> index(k, 0);
    QBL b(k, 10);
    b.choose(m);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(index, reward);
        }
    }
}
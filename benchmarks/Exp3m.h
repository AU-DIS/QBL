static void benchmark_exp3m(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    int rounds = state.range(2);

    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }

    Exp3m b(m, k, 0.1);

    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            auto choices = b.choose(m);
            b.give_reward(choices, reward);
        }
    }
}

static void benchmark_exp3m_sample(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }

    int rounds = state.range(2);
    Exp3m b(m, k, 0.1);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            benchmark::DoNotOptimize(b.choose(m));
        }
    }
}

static void benchmark_exp3m_update(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }
    int rounds = state.range(2);

    Exp3m b(m, k, 0.1);
    b.choose(m);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(index, reward);
        }
    }
}

static void benchmark_fplm(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }

    int rounds = state.range(2);
    FPL b(k, 10);

    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            auto choices = b.choose(m);
            b.give_reward(choices, reward);
        }
    }
}

static void benchmark_fplm_sample(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }

    int rounds = state.range(2);
    FPL b(k, 10);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            benchmark::DoNotOptimize(b.choose(m));
        }
    }
}

static void benchmark_fplm_update(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }

    int rounds = state.range(2);
    FPL b(k, 10);

    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(index, reward);
        }
    }
}


static void benchmark_exp3m_heap(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    int rounds = state.range(2);

    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }


    Exp3m_heap b(m, k, 0.1);

    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            auto choices = b.choose(m);
            b.give_reward(choices, reward);
        }
    }
}

static void benchmark_exp3m_heap_sample(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    int rounds = state.range(2);

    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }


    Exp3m_heap b(m, k, 0.1);
    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            benchmark::DoNotOptimize(b.choose(m));
        }
    }
}

static void benchmark_exp3m_heap_update(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    int rounds = state.range(2);

    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }
    Exp3m_heap b(m, k, 0.1);

    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            b.give_reward(index, reward);
        }
    }
}

static void benchmark_qblm(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    int rounds = state.range(2);

    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }


    QBL b(k, 0.1);

    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            auto choices = b.choose(m);
            b.give_reward(choices, reward);
        }
    }
}

static void benchmark_qblm_sample(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    int rounds = state.range(2);

    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }


    QBL b(k, 0.1);

    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            auto choices = b.choose(m);
            //b.give_reward(choices, reward);
        }
    }
}

static void benchmark_qblm_update(benchmark::State &state)
{
    int k = state.range(0);
    int m = state.range(1);
    int rounds = state.range(2);

    std::vector<double> reward(k, 0);
    std::vector<int> index(m, 0);
    for (int i = 0; i < m; i++) {
        reward[i] = i%3/3.0;
        index[i] = i;
    }


    QBL b(k, 0.1);

    for (auto _ : state)
    {
        for (int i = 0; i < rounds; i++)
        {
            //auto choices = b.choose(m);
            b.give_reward(index, reward);
        }
    }
}
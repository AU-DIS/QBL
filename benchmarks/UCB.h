
static void benchmark_ucb_fpl(benchmark::State &state)
{
    auto k = state.range(0);
    FPL bandit(k, 0.1);
    UCB1 b(1, bandit);
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

static void benchmark_ucb_exp3(benchmark::State &state)
{
    auto k = state.range(0);
    Exp3 bandit(k, 0.1);
    UCB1 b(1, bandit);
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
#include "experiments/DuelArena.h"
#include "algorithms/FPL/FPL.h"
#include "algorithms/Exp3Bandit/Exp3.h"

int main(int argc, char *argv[])
{
    int k = 64;
    int rounds = 100000;

    //auto q = FPL(k, 10);
    //auto g = FPL(k, 10);
    auto g = Exp3(k, 0.01);
    auto q = Exp3(k, 0.01);
    run_bandit_duel(q, g, k, rounds);
}
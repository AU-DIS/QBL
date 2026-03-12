
#include "iostream"
#include <benchmark/benchmark.h>
#include "random"
#include "algorithms/FPL/FPL.h"
#include "algorithms/FPL/QBL.h"
//#include "algorithms/FPL/FPL_hash.h"
//#include "algorithms/Exp3Bandit/Exp31.h"
//#include "algorithms/Exp3Bandit/Exp3.1_optimized.h"
#include "algorithms/Exp3Bandit/Exp3.h"
//#include "algorithms/Tsallis-INF/TsallisINF.h"
//#include "algorithms/Tsallis-INF/Tsallis_RV.h"
//#include "algorithms/Tsallis-INF/Tsallis_IW.h"
//#include "algorithms/Tsallis-INF/Tsallis_LTU.h"
//#include "algorithms/Tsallis-INF/Tsallis_approx_rv.h"
//#include "algorithms/Tsallis-INF/Tsallis_Heap.h"
//#include "algorithms/DEXP/DExp3.h"
#include "algorithms/Exp3m/DepRoundALIASStrategy.h"
#include "algorithms/Exp3m/Exp3m.h"
#include "algorithms/Exp3m/Exp3m_heap.h"
#include "algorithms/Exp3m/Exp31m.h"
//#include "algorithms/FPL/FPL_buckets.h"
//#include "algorithms/FPL/FPL_toplog.h"
//#include "algorithms/UCB/UCB1.h"
//#include "algorithms/Exp3Bandit/Exp3Tor.h"
#include "algorithms/Exp3Bandit/Exp3_heap.h"
//#include "algorithms/Exp3Bandit/Exp3_fid.h"
//#include "algorithms/Exp3Bandit/Exp3_lad.h"
#include "experiments/AdversarialExperimentRunner.h"
//#include "datasets/data_generators.h"
#include "algorithms/Exp3Bandit/Exp3Light.h"
#include "algorithms/Exp3Bandit/Exp3LightHeap.h"
#include "algorithms/Exp3Bandit/Exp3IXTor.h"

#include "benchmarks/Policy.h"
//Single arm benchmarks (second param is ignored for all but FPL)
BENCHMARK(benchmark_policy<Exp3>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<Exp3>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<Exp3>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_policy<FPL>)->Args({100,100})->Args({1000,100})->Args({10000,100})->Args({100000,100})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<FPL>)->Args({100,100})->Args({1000,100})->Args({10000,100})->Args({100000,100})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<FPL>)->Args({100,100})->Args({1000,100})->Args({10000,100})->Args({100000,100})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_policy<Exp3Light>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<Exp3Light>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<Exp3Light>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_policy<Exp3IXTor>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<Exp3IXTor>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<Exp3IXTor>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_policy<Exp3_heap>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<Exp3_heap>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<Exp3_heap>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_policy<Exp3LightHeap>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<Exp3LightHeap>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<Exp3LightHeap>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_policy<QBL>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<QBL>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<QBL>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime()->Unit(benchmark::kMillisecond);


//Scaling m with k
BENCHMARK(benchmark_m_policy<Exp3m>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<Exp3m>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<Exp3m>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_m_policy<FPL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<FPL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<FPL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_m_policy<Exp3m_heap>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<Exp3m_heap>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<Exp3m_heap>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_m_policy<QBL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<QBL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<QBL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);

// Fixed k
BENCHMARK(benchmark_m_policy<Exp3m>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<Exp3m>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<Exp3m>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_m_policy<FPL>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<FPL>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<FPL>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_m_policy<Exp3m_heap>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<Exp3m_heap>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<Exp3m_heap>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_m_policy<QBL>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<QBL>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<QBL>)->Args({2000,2})->Args({2000,20})->Args({2000,200})->Args({2000,2000})->Iterations(50)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
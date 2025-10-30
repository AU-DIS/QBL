
#include "iostream"
#include <benchmark/benchmark.h>
#include "random"
#include "algorithms/FPL/FPL.h"
#include "algorithms/FPL/QBL.h"
#include "algorithms/FPL/FPL_hash.h"
#include "algorithms/Exp3Bandit/Exp31.h"
#include "algorithms/Exp3Bandit/Exp3.1_optimized.h"
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
#include "algorithms/FPL/FPL_buckets.h"
#include "algorithms/FPL/FPL_toplog.h"
#include "algorithms/UCB/UCB1.h"
#include "algorithms/Exp3Bandit/Exp3Tor.h"
#include "algorithms/Exp3Bandit/Exp3_heap.h"
#include "algorithms/Exp3Bandit/Exp3_fid.h"
#include "algorithms/Exp3Bandit/Exp3_lad.h"
#include "experiments/AdversarialExperimentRunner.h"
#include "datasets/data_generators.h"
#include "algorithms/Exp3Bandit/Exp3Light.h"
#include "algorithms/Exp3Bandit/Exp3LightHeap.h"
#include "algorithms/Exp3Bandit/Exp3IXTor.h"

#include "benchmarks/Exp3_dum.h"
#include "benchmarks/Exp3_eta.h"
#include "benchmarks/Exp3_heap.h"
#include "benchmarks/Exp3.1.h"
#include "benchmarks/Exp3.h"
#include "benchmarks/Exp3m.h"
#include "benchmarks/FPL_H.h"
#include "benchmarks/FPL.h"
#include "benchmarks/FPL_buckets.h"
#include "benchmarks/FPL_toplog.h"
#include "benchmarks/QBL.h"
#include "benchmarks/exp3light.h"
#include "benchmarks/exp3lightheap.h"
#include "benchmarks/Exp3ixtor.h"
//#include "benchmarks/Tsallis_iw.h"
//#include "benchmarks/Tsallis_LTU.h"
//#include "benchmarks/Tsallis_approx_rv.h"
//#include "benchmarks/Tsallis_rv.h"
#include "benchmarks/UCB.h"
#include "benchmarks/Policy.h"
//BENCHMARK(benchmark_policy<Exp3>)->Args({100,(long)0.1})->Args({1000,(long)0.1})->Args({10000,(long)0.1})->Args({100000,(long)0.1})->Threads(10)->Unit(benchmark::kMillisecond);
/*
BENCHMARK(benchmark_policy<Exp3>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<Exp3>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<Exp3>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_policy<FPL>)->Args({100,100})->Args({1000,100})->Args({10000,100})->Args({100000,100})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<FPL>)->Args({100,100})->Args({1000,100})->Args({10000,100})->Args({100000,100})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<FPL>)->Args({100,100})->Args({1000,100})->Args({10000,100})->Args({100000,100})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
//ssample supdate reversed for fpl

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

BENCHMARK(benchmark_policy<QBL>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime();//->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_sample<QBL>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime();//->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_policy_update<QBL>)->Args({100,1})->Args({1000,1})->Args({10000,1})->Args({100000,1})->UseManualTime();//->Unit(benchmark::kMillisecond);
*/
BENCHMARK(benchmark_m_policy<Exp3m>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<Exp3m>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<Exp3m>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_m_policy<FPL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<FPL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<FPL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_m_policy<Exp3m_heap>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<Exp3m_heap>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<Exp3m_heap>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_m_policy<QBL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_sample<QBL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_m_policy_update<QBL>)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Iterations(10)->UseManualTime()->Unit(benchmark::kMillisecond);




/*
BENCHMARK(benchmark_exp3)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_fpl)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_fpl_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_fpl_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_exp3_light)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_light_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_light_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_exp3_ix)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_ix_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_ix_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_exp3_heap)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_heap_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_heap_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_exp3_light_heap)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_light_heap_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3_light_heap_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_qbl)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_qbl_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_qbl_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8)->Unit(benchmark::kMillisecond);
*/
/*
BENCHMARK(benchmark_exp3m)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3m_sample)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3m_update)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_fplm)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_fplm_sample)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_fplm_update)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_exp3m_heap)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3m_heap_sample)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3m_heap_update)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_qblm)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_qblm_sample)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_qblm_update)->Args({100,100/10})->Args({1000,1000/10})->Args({10000,10000/10})->Args({100000,100000/10})->Threads(10)->Unit(benchmark::kMillisecond);
*/
/*
int rounds = 10000;
BENCHMARK(benchmark_exp3m)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3m_sample)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3m_update)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_fplm)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_fplm_sample)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_fplm_update)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_exp3m_heap)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3m_heap_sample)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_exp3m_heap_update)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);

BENCHMARK(benchmark_qblm)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_qblm_sample)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_qblm_update)->Args({2000,2, rounds})->Args({2000,20, rounds})->Args({2000,200, rounds})->Args({2000,2000, rounds})->Threads(10)->Unit(benchmark::kMillisecond);
*/








//BENCHMARK(benchmark_fpl_toplog)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(1)->Unit(benchmark::kMillisecond);
//BENCHMARK(benchmark_fpl_toplog_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
//BENCHMARK(benchmark_fpl_toplog_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);

//BENCHMARK(benchmark_fpl_hashing)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(1);
//BENCHMARK(benchmark_fpl_hashing_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(1);
//BENCHMARK(benchmark_fpl_hashing_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(1);

/*
BENCHMARK(benchmark_exp3_fid)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp3_fid_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp3_fid_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);

BENCHMARK(benchmark_exp3_lad)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp3_lad_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp3_lad_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
*/
/*
BENCHMARK(benchmark_exp31)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp31_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp31_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);

BENCHMARK(benchmark_exp31_optimized)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp31_optimized_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp31_optimized_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);


BENCHMARK(benchmark_exp31)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp31_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_exp31_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);

BENCHMARK(benchmark_tsallis_iw)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_iw_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_iw_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);

BENCHMARK(benchmark_tsallis_rv)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_rv_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_rv_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);

BENCHMARK(benchmark_tsallis_ltu)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_ltu_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_ltu_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);

BENCHMARK(benchmark_tsallis_approxrv)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_approxrv_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_approxrv_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);

BENCHMARK(benchmark_tsallis_heap)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_heap_sample)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
BENCHMARK(benchmark_tsallis_heap_update)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Threads(8);
*/

BENCHMARK_MAIN();
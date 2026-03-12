# Practical Adversarial Multi-Armed Bandits with Sublinear Runtime

# Artifact Evaluation
This repository contains all code for the project. Here we discuss how to produce the outputs for the main results of the evaluation (Figures 4,5,6,7)

### Setup
The latest test of the setup was on Ubuntu 24.04.4 LTS x86_64.
For plotting, common python packages such as `pandas` and `matplotlib` is required.
For compiling, two other projects must be cloned into this one. Steps provided below.

To build the project:
```bash
$ git clone git@github.com:AU-DIS/QBL.git
$ cd QBL
# Download dependencies
# Boost
$ sudo apt-get install libboost-all-dev
# csv-parser (in project root folder)
$ git clone git@github.com:vincentlaucsb/csv-parser.git 
# Google Benchmark (in project folder)
$ git clone https://github.com/google/benchmark.git
$ cd benchmark
$ cmake -E make_directory "build"
$ cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release -S . -B "build"
$ cmake --build "build" --config Release
$ cd ..
# Build this project
$ cmake -DCMAKE_BUILD_TYPE=Release -S .
$ cmake --build .
# Allow executable scripts
$ sudo chmod +x *.sh
```

### Runtime benchmarks (Figure 6 and 7 )
```bash
$ ./run_benchmark.sh
```
An output from Google Benchmark will be printed to the terminal in the style of:
```
----------------------------------------------------------------------------------------------------------------
Benchmark                                                                      Time             CPU   Iterations
----------------------------------------------------------------------------------------------------------------
benchmark_m_policy<QBL>/100/10/iterations:50/manual_time                   0.591 ms        0.664 ms           50
benchmark_m_policy<QBL>/1000/100/iterations:50/manual_time                  4.54 ms         4.69 ms           50
benchmark_m_policy<QBL>/10000/1000/iterations:50/manual_time                66.8 ms         67.9 ms           50
benchmark_m_policy<QBL>/100000/10000/iterations:50/manual_time               763 ms          774 ms           50
benchmark_m_policy_sample<QBL>/100/10/iterations:50/manual_time            0.217 ms        0.460 ms           50
benchmark_m_policy_sample<QBL>/1000/100/iterations:50/manual_time           2.77 ms         4.89 ms           50
benchmark_m_policy_sample<QBL>/10000/1000/iterations:50/manual_time         47.9 ms         69.4 ms           50
benchmark_m_policy_sample<QBL>/100000/10000/iterations:50/manual_time        494 ms          784 ms           50
benchmark_m_policy_update<QBL>/100/10/iterations:50/manual_time            0.230 ms        0.462 ms           50
benchmark_m_policy_update<QBL>/1000/100/iterations:50/manual_time           2.04 ms         4.77 ms           50
benchmark_m_policy_update<QBL>/10000/1000/iterations:50/manual_time         20.5 ms         67.1 ms           50
benchmark_m_policy_update<QBL>/100000/10000/iterations:50/manual_time        290 ms          771 ms           50
``` 

And the `csv` files `combined.csv`, `samlpe.csv`,`update.csv` will be produced, containing compacted overviews.  
Example of `sample.csv`:
```
name,k,m,iterations,real_time,cpu_time,time_unit
QBL,100,10,50,0.216797,0.459572,ms
QBL,1000,100,50,2.76651,4.89428,ms
QBL,10000,1000,50,47.8559,69.4431,ms
QBL,100000,10000,50,493.791,784.167,ms
```

A full set of outputfiles can be found in the folder `example_outputs/`

> [!NOTE]
> The column `Iterations` refeers to the number of executions of the function and not the timehorizon of the algorithms. The timehorizon is set to 1000 for these benchmarks.

real_time, measures the runtime of the targeted routine, while cpu_time measures the total execution time of the bandit. Sample and update times are measured independendly from the total time, hence cpu_time should be simmilar between the sample, update and total time measues of an algorithm to ensure they are comparable with each other. This should only be a concern for fast methods or with small parameter settings.    


## Regret benchmarks (Figure 4 and 5)
```bash
$ ./run_generic.sh
```
This script will generate all regret plots and data. **There are many file** so as standard they are saved to your root `/tmp` folder.

The `run_generic.sh` script calls  
`experiments/experiment_specifications/AdversarialExperiments/run_adversarial_exp3m.sh`  
and    
`experiments/experiment_specifications/AdversarialExperiments/run_adversarial_fpl.sh`  

Where the output folder can be modified.

> [!NOTE]
> These experiments can be very slow to run with the full `T=100000`. We have set it to 1000 in this example, but it can be changed in the above scripts to reproduce the complete data in Figure 4 and 5. Doing so is needed to observe the real trends. 

Be aware multiple sub routines return prints in the terminal after the main process has closed, so the script may have finished, even though it does not look like it. Check `htop` if in doubt.
  


___
# Bonus 
___
> [!IMPORTANT]
> The following section describes the setup for results shown in figure 2 and 3. They require the availability on an mssql server and notable RAM and storage.
These are supporting experiments.
## Index tuning experiment: DBABandits-modded & HMAB-modded
A modified version of https://github.com/malingaperera/DBABandits with the addition of QBL


- Online Graph summarisation experiment: See folder AGLIMPSE
- Index tuning experiment: See folder DBABandits-modded and HMAB-modded

## The setup 
The setup requires a connected mssql TPC-H server to run. We provide an overview of a setup using microsofts provided docker image.

Pull mssql container with  
`sudo docker pull mopt/mssql-tools/bin/scr.microsoft.com/mssql/server:2022-latest`

Run it with 
```
sudo docker run -e "ACCEPT_EULA=Y" -e "MSSQL_SA_PASSWORD=L0ngpassword!" \
-it --mount src="~/path/to/schema",target=/mnt,type=bind \
   -p 1433:1433 --name sql1 --hostname sql1 \
   -d \
   mcr.microsoft.com/mssql/server:2022-latest
```

Which mounts a path that can be used to save table files into. Remember to set a suitable password.

Create schema files with TPC-H generator. One way is outlined here: https://github.com/nghiahhnguyen/TPC-H-Dataset-Generator-MS-SQL-Server It may require minor modification of the bulkloading and .ddl and .ri files depending on setup.

An easy way to access the is with **`mssql-cli`**

You can then connect with `mssql-cli -S localhost -U SA -P L0ngpassword!` and run sql commands e.g. `DROP DATABASE tpc` and `CREATE DATABASE tpc`

With a database created you can run files such as 
`mssql-cli -S localhost -U SA -P L0ngpassword! -d tpc -i dss.ddl & mssql-cli -S localhost -U SA -P L0ngpassword! -d tpc -i dss.ri`

Which can reference schema table files from the mounted `/mnt` folder

# Online Graph summarisation experiment A-glimpse:
This repository contains experimental setups to compare bandit approaches against Glimpse for online knowledgegraph summarisation.

## Setup

1. Install Python packages from requiriements.txt
2. Download dbpedia39 
    
    2.1 run `downLoadDBPedia39()` from **`util.py`** to download the requiried rdf files into folder **`dbpedia/rdf_gz/`**

    2.3 outcomment `downLoadDBPedia39()` and uncomment the graph load and save instead. Run it. 
    
    2.2 run **`subgraphs.py`** to create a simple smaller test graph for quick experimentation. Its generation can freely be modified.

    2.3 if any step fail. You may have to update the folder path to **`dbpedia/rdf_gz`** in both  **`glimpse/src/experiment_base.py`** and **`glimpse/src/base.py`** for `class DBPedia(KnowledgeGraph):`.

4. run **`main.py`** and check if it works! 

5. Plots are saved in **`replacements_results/`**. Beware that a lot of naming is legacy stats that may or may not mean anything.

## How, where and what to modify
There is 4 things of interest to modify.
* The methods
* The parameters
* The queries
* The graphs

**The Methods**

Bandits are saved ins **`bandits/effecients_bandits/`**.
The Exp3 variants are older implementations that I may update.
Changing bandits is currently not that flexible and has to be done manually in `run_compare_experiment()` line ~60 by changing between `bandit="exp3"` and `bandit="qbl"`. To make a new bandit simply create a new file and implement the same methods. Then add it in **`glimpseonline.py`** like the other bandits. 
> **Note** All of this method swapping can be automated. 

**The Parameters**

Bandit parameters are currently set individually and very unorganised. 

Most Experiment parameters can be found in **`main.py`**.
You can execute multiple experiments at once multi threaded (See below). For now use only `run_compares()` as other methods contain legacy.

`graph`: name of graph to load.\
`graph_size`: custom defined size of the graph (LEGACY when using `run_compares()`)\
`n`: number of timesteps.\
`ks`: target size of summary graph proportional to size of the full graph.\
`batch_size`: number of queries provided in each timestep.\
`query_generator`: name of query generator to use.

`ks, batch_size, query_Generator` is wrapped in a list, allowing to execute experiments with multiple settings at once.

In **`experiments/bandit_versus_glimpse.py`** the variable `number_of_aggregations` can be set to make an average result of each experiment. `number_of_aggregations=1` executes each experiment once. `number_of_aggregations=10` executes each experiment 10 times and report the average.

**The graph**

All experiments are based on dbpedia or subgraphs thereof.
subgraphs can be generated. See examples of how in **`subgraphs.py`**. Things to consider: to generate some more meaningfull graphs, it would be smart to utilize graph traversal, otherwise they become very disconected. Though that is fine in some cases e.g. special query loads or just for simple code experimentation. 



## Multithreaded experiments

To run multiple experiments in parallelle, simply uncomment
```Python
                    #p = Process(target=run_compare_experiment,
                    #            args=(graph, n, k, bs, rf, query_generator))
                    #processes.append(p)

    #for p in processes:
        #Too much parralell if splitting this
        #p.start()
        #p.join()
```
in `run_compares()` and remove the old `run_compare_experiment()` line above it.




# Dependencies
- https://github.com/vincentlaucsb/csv-parser?ref=bestofcpp.com#integration
- Google benchmark
- boost

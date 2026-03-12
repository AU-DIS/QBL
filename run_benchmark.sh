#!/usr/bin/bash

# Clean up csv files
rm *.csv
#rm /tmp/bandits/*.csv 2>/dev/null
# Run benchmarks and process them
./benchmark_bandits --benchmark_out=raw_out.csv --benchmark_out_format=csv #--benchmark_repetitions=10
#valgrind --tool=callgrind --trace-children=yes ./benchmark_bandits --benchmark_out=raw_out.csv --benchmark_out_format=csv #--benchmark_repetitions=10
./process_benchmarks.sh raw_out.csv out.csv
# Prepare output file swith headers
echo "name,k,m,iterations,real_time,cpu_time,time_unit" >> header.csv

# Filter basic algorithms
#rm /tmp/bandits/*.csv 2>/dev/null

./filter_basic_benchmarks.sh .



python3 plotting/benchmark.py sample.csv sample.png
python3 plotting/benchmark.py update.csv update.png
python3 plotting/benchmark.py combined.csv combined.png

#./filter_fpl_benchmarks.sh /tmp /mnt/ssd/bandits
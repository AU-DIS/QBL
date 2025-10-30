#!/bin/bash

sed 's/exp31/Exp31/g' $1 |
    sed 's/exp3/Exp3/g' |
    sed 's/_defer/ (defer)/g' |
    sed 's/_fid/ FID/g' |
    sed 's/_lad/ LAD/g' |
    sed 's/name,/name,k,/g' |
    sed 's/"//g' |
    sed 's/benchmark_//g' |
    sed 's/\/threads:15//g' |
    sed 's/\/threads:8//g' |
    sed 's/\/threads:1//g' |
    sed 's/\/1/,1/g' |
    sed 's/_sample/ (sample)/g' |
    sed 's/_update/ (update)/g' |
    sed 's/qbl/QBL/g' |
    sed 's/fpl/FPL/g' |
    sed 's/tsallis/Tsallis/g' |
    sed 's/Tsallis_heap/Cumulative loss sampling/g' |
    sed 's/_optimized/ Optimized/g' |
    sed 's/_bucket/ (bucket)/g' |
    sed 's/_toplog/ (toplog)/g' |
    sed 's/_hashing/ (hashing)/g' |
    sed 's/_rv/ (RV)/g' |
    sed 's/_iw/ (IW)/g' |
    sed 's/_ltu/ (LTU)/g' |
    sed 's/_approxrv/ (RV approx)/g' |
    sed 's/_heap/ Heap/g' >>$2

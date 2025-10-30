#!/usr/bin/bash
$tmp_dir=$1

grep 'Exp3,' out.csv >> combined.csv
grep 'Exp3 (sample)' out.csv >> basic_sample.csv
grep 'Exp3 (update)' out.csv >> basic_update.csv

grep 'Exp31,' out.csv >> combined.csv
grep 'Exp31 (sample)' out.csv >> basic_sample.csv
grep 'Exp31 (update)' out.csv >> basic_update.csv

grep 'FPL,' out.csv >> combined.csv
grep 'FPL (sample)' out.csv >> basic_sample.csv
grep 'FPL (update)' out.csv >> basic_update.csv

grep 'Tsallis (IW),' out.csv >> combined.csv
grep 'Tsallis (IW) (sample)' out.csv >> basic_sample.csv
grep 'Tsallis (IW) (update)' out.csv >> basic_update.csv

grep 'Tsallis (RV),' out.csv >> combined.csv
grep 'Tsallis (RV) (sample)' out.csv >> basic_sample.csv
grep 'Tsallis (RV) (update)' out.csv >> basic_update.csv

grep "sample" basic.csv >> sample.csv
grep "update" basic.csv >> update.csv

sed 's/ (sample)//g' basic_sample.csv >> sample_out.csv
sed 's/ (update)//g' basic_update.csv >> update_out.csv

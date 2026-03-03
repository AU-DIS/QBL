#!/usr/bin/bash
#$tmp_dir=$1


grep 'QBL' out.csv >> combined_out.csv
grep 'FPL' out.csv >> combined_out.csv
grep 'sample_' out.csv >> basic_sample.csv
grep 'update_' out.csv >> basic_update.csv


sed 's/sample_//g' basic_sample.csv >> sample_out.csv
sed 's/update_//g' basic_update.csv >> update_out.csv


cat header.csv sample_out.csv >> sample.csv
cat header.csv update_out.csv >> update.csv
cat header.csv combined_out.csv >> combined.csv
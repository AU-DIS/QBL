rm /tmp/*.csv
name=FPL
tmp_dir=$1
destination_dir=$2
echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/FPL_header.csv

# Top-log comparison
grep 'FPL,' out.csv >>$tmp_dir/FPL_combined.csv
grep 'FPL (sample)' out.csv >>$tmp_dir/FPL_sample.csv
grep 'FPL (update)' out.csv >>$tmp_dir/FPL_update.csv

grep 'FPL (toplog),' out.csv >>$tmp_dir/FPL_combined.csv
grep 'FPL (toplog) (sample)' out.csv >>$tmp_dir/FPL_sample.csv
grep 'FPL (toplog) (update)' out.csv >>$tmp_dir/FPL_update.csv

sed 's/ (sample)//g' $tmp_dir/FPL_sample.csv >>$tmp_dir/FPL_sample_out.csv
sed 's/ (update)//g' $tmp_dir/FPL_update.csv >>$tmp_dir/FPL_update_out.csv

cat $tmp_dir/FPL_header.csv $tmp_dir/FPL_sample_out.csv >>$tmp_dir/FPL_sample_done.csv
cat $tmp_dir/FPL_header.csv $tmp_dir/FPL_update_out.csv >>$tmp_dir/FPL_update_done.csv
cat $tmp_dir/FPL_header.csv $tmp_dir/FPL_combined.csv >>$tmp_dir/FPL_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/FPL_sample_done.csv $destination_dir/toplog_sample.png
python3 plotting/benchmark.py $tmp_dir/FPL_update_done.csv $destination_dir/toplog_update.png
python3 plotting/benchmark.py $tmp_dir/FPL_combined_done.csv $destination_dir/toplog_combined.png

# Clean
rm $tmp_dir/*.csv

# QBL
echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/FPL_header.csv
# Top-log comparison
grep 'FPL,' out.csv >>$tmp_dir/FPL_combined.csv
grep 'FPL (sample)' out.csv >>$tmp_dir/FPL_sample.csv
grep 'FPL (update)' out.csv >>$tmp_dir/FPL_update.csv

grep 'QBL,' out.csv >>$tmp_dir/FPL_combined.csv
grep 'QBL (sample)' out.csv >>$tmp_dir/FPL_sample.csv
grep 'QBL (update)' out.csv >>$tmp_dir/FPL_update.csv

sed 's/ (sample)//g' $tmp_dir/FPL_sample.csv >>$tmp_dir/FPL_sample_out.csv
sed 's/ (update)//g' $tmp_dir/FPL_update.csv >>$tmp_dir/FPL_update_out.csv

cat $tmp_dir/FPL_header.csv $tmp_dir/FPL_sample_out.csv >>$tmp_dir/FPL_sample_done.csv
cat $tmp_dir/FPL_header.csv $tmp_dir/FPL_update_out.csv >>$tmp_dir/FPL_update_done.csv
cat $tmp_dir/FPL_header.csv $tmp_dir/FPL_combined.csv >>$tmp_dir/FPL_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/FPL_sample_done.csv $destination_dir/qbl_sample.png
python3 plotting/benchmark.py $tmp_dir/FPL_update_done.csv $destination_dir/qbl_update.png
python3 plotting/benchmark.py $tmp_dir/FPL_combined_done.csv $destination_dir/qbl_combined.png

# Clean
rm $tmp_dir/*.csv

# Hashing
echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/FPL_header.csv

grep 'FPL,' out.csv >>$tmp_dir/FPL_combined.csv
grep 'FPL (sample)' out.csv >>$tmp_dir/FPL_sample.csv
grep 'FPL (update)' out.csv >>$tmp_dir/FPL_update.csv

grep 'FPL (hashing),' out.csv >>$tmp_dir/FPL_combined.csv
grep 'FPL (hashing) (sample)' out.csv >>$tmp_dir/FPL_sample.csv
grep 'FPL (hashing) (update)' out.csv >>$tmp_dir/FPL_update.csv

sed 's/ (sample)//g' $tmp_dir/FPL_sample.csv >>$tmp_dir/FPL_sample_out.csv
sed 's/ (update)//g' $tmp_dir/FPL_update.csv >>$tmp_dir/FPL_update_out.csv

cat $tmp_dir/FPL_header.csv $tmp_dir/FPL_sample_out.csv >>$tmp_dir/FPL_sample_done.csv
cat $tmp_dir/FPL_header.csv $tmp_dir/FPL_update_out.csv >>$tmp_dir/FPL_update_done.csv
cat $tmp_dir/FPL_header.csv $tmp_dir/FPL_combined.csv >>$tmp_dir/FPL_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/FPL_sample_done.csv $destination_dir/fpl_hashing_sample.png
python3 plotting/benchmark.py $tmp_dir/FPL_update_done.csv $destination_dir/fpl_hashing_update.png
python3 plotting/benchmark.py $tmp_dir/FPL_combined_done.csv $destination_dir/fpl_hashing_combined.png

# Clean
rm $tmp_dir/*.csv

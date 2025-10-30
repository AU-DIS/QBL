rm /tmp/*.csv
name=Tsallis
tmp_dir=$1
destination_dir=$2
echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/Tsallis_header.csv

# LTU
grep 'Tsallis (IW),' out.csv >>$tmp_dir/Tsallis_combined.csv
grep 'Tsallis (IW) (sample)' out.csv >>$tmp_dir/Tsallis_sample.csv
grep 'Tsallis (IW) (update)' out.csv >>$tmp_dir/Tsallis_update.csv

grep 'Tsallis (LTU),' out.csv >>$tmp_dir/Tsallis_combined.csv
grep 'Tsallis (LTU) (sample)' out.csv >>$tmp_dir/Tsallis_sample.csv
grep 'Tsallis (LTU) (update)' out.csv >>$tmp_dir/Tsallis_update.csv

sed 's/ (sample)//g' $tmp_dir/Tsallis_sample.csv >>$tmp_dir/Tsallis_sample_out.csv
sed 's/ (update)//g' $tmp_dir/Tsallis_update.csv >>$tmp_dir/Tsallis_update_out.csv

cat $tmp_dir/Tsallis_header.csv $tmp_dir/Tsallis_sample_out.csv >>$tmp_dir/Tsallis_sample_done.csv
cat $tmp_dir/Tsallis_header.csv $tmp_dir/Tsallis_update_out.csv >>$tmp_dir/Tsallis_update_done.csv
cat $tmp_dir/Tsallis_header.csv $tmp_dir/Tsallis_combined.csv >>$tmp_dir/Tsallis_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/Tsallis_sample_done.csv $destination_dir/ltu_sample.png
python3 plotting/benchmark.py $tmp_dir/Tsallis_update_done.csv $destination_dir/ltu_update.png
python3 plotting/benchmark.py $tmp_dir/Tsallis_combined_done.csv $destination_dir/ltu_combined.png

# Clean
rm $tmp_dir/*.csv

echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/Tsallis_header.csv

# Approx RV
grep 'Tsallis (RV),' out.csv >>$tmp_dir/Tsallis_combined.csv
grep 'Tsallis (RV) (sample)' out.csv >>$tmp_dir/Tsallis_sample.csv
grep 'Tsallis (RV) (update)' out.csv >>$tmp_dir/Tsallis_update.csv

grep 'Tsallis (RV approx),' out.csv >>$tmp_dir/Tsallis_combined.csv
grep 'Tsallis (RV approx) (sample)' out.csv >>$tmp_dir/Tsallis_sample.csv
grep 'Tsallis (RV approx) (update)' out.csv >>$tmp_dir/Tsallis_update.csv

sed 's/ (sample)//g' $tmp_dir/Tsallis_sample.csv >>$tmp_dir/Tsallis_sample_out.csv
sed 's/ (update)//g' $tmp_dir/Tsallis_update.csv >>$tmp_dir/Tsallis_update_out.csv

cat $tmp_dir/Tsallis_header.csv $tmp_dir/Tsallis_sample_out.csv >>$tmp_dir/Tsallis_sample_done.csv
cat $tmp_dir/Tsallis_header.csv $tmp_dir/Tsallis_update_out.csv >>$tmp_dir/Tsallis_update_done.csv
cat $tmp_dir/Tsallis_header.csv $tmp_dir/Tsallis_combined.csv >>$tmp_dir/Tsallis_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/Tsallis_sample_done.csv $destination_dir/approxrv_sample.png
python3 plotting/benchmark.py $tmp_dir/Tsallis_update_done.csv $destination_dir/approxrv_update.png
python3 plotting/benchmark.py $tmp_dir/Tsallis_combined_done.csv $destination_dir/approxrv_combined.png

# Clean
rm $tmp_dir/*.csv

echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/Tsallis_header.csv

# Approx RV
grep 'Tsallis (RV),' out.csv >>$tmp_dir/Tsallis_combined.csv
grep 'Tsallis (RV) (sample)' out.csv >>$tmp_dir/Tsallis_sample.csv
grep 'Tsallis (RV) (update)' out.csv >>$tmp_dir/Tsallis_update.csv

grep 'Cumulative loss sampling,' out.csv >>$tmp_dir/Tsallis_combined.csv
grep 'Cumulative loss sampling (sample)' out.csv >>$tmp_dir/Tsallis_sample.csv
grep 'Cumulative loss sampling (update)' out.csv >>$tmp_dir/Tsallis_update.csv

sed 's/ (sample)//g' $tmp_dir/Tsallis_sample.csv >>$tmp_dir/Tsallis_sample_out.csv
sed 's/ (update)//g' $tmp_dir/Tsallis_update.csv >>$tmp_dir/Tsallis_update_out.csv

cat $tmp_dir/Tsallis_header.csv $tmp_dir/Tsallis_sample_out.csv >>$tmp_dir/Tsallis_sample_done.csv
cat $tmp_dir/Tsallis_header.csv $tmp_dir/Tsallis_update_out.csv >>$tmp_dir/Tsallis_update_done.csv
cat $tmp_dir/Tsallis_header.csv $tmp_dir/Tsallis_combined.csv >>$tmp_dir/Tsallis_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/Tsallis_sample_done.csv $destination_dir/heap_sample.png
python3 plotting/benchmark.py $tmp_dir/Tsallis_update_done.csv $destination_dir/heap_update.png
python3 plotting/benchmark.py $tmp_dir/Tsallis_combined_done.csv $destination_dir/heap_combined.png

# Clean
rm $tmp_dir/*.csv

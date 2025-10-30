rm /tmp/*.csv
name=Exp3
tmp_dir=$1
destination_dir=$2
echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/Exp3_header.csv

# FID
grep 'Exp3,' out.csv >>$tmp_dir/Exp3_combined.csv
grep 'Exp3 (sample)' out.csv >>$tmp_dir/Exp3_sample.csv
grep 'Exp3 (update)' out.csv >>$tmp_dir/Exp3_update.csv

grep 'Exp3 FID,' out.csv >>$tmp_dir/Exp3_combined.csv
grep 'Exp3 FID (sample)' out.csv >>$tmp_dir/Exp3_sample.csv
grep 'Exp3 FID (update)' out.csv >>$tmp_dir/Exp3_update.csv

sed 's/ (sample)//g' $tmp_dir/Exp3_sample.csv >>$tmp_dir/Exp3_sample_out.csv
sed 's/ (update)//g' $tmp_dir/Exp3_update.csv >>$tmp_dir/Exp3_update_out.csv

cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_sample_out.csv >>$tmp_dir/Exp3_sample_done.csv
cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_update_out.csv >>$tmp_dir/Exp3_update_done.csv
cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_combined.csv >>$tmp_dir/Exp3_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/Exp3_sample_done.csv $destination_dir/fid_sample.png
python3 plotting/benchmark.py $tmp_dir/Exp3_update_done.csv $destination_dir/fid_update.png
python3 plotting/benchmark.py $tmp_dir/Exp3_combined_done.csv $destination_dir/fid_combined.png

# Clean
rm $tmp_dir/*.csv


echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/Exp3_header.csv

# FID
grep 'Exp3,' out.csv >>$tmp_dir/Exp3_combined.csv
grep 'Exp3 (sample)' out.csv >>$tmp_dir/Exp3_sample.csv
grep 'Exp3 (update)' out.csv >>$tmp_dir/Exp3_update.csv

grep 'Exp3 LAD,' out.csv >>$tmp_dir/Exp3_combined.csv
grep 'Exp3 LAD (sample)' out.csv >>$tmp_dir/Exp3_sample.csv
grep 'Exp3 LAD (update)' out.csv >>$tmp_dir/Exp3_update.csv

sed 's/ (sample)//g' $tmp_dir/Exp3_sample.csv >>$tmp_dir/Exp3_sample_out.csv
sed 's/ (update)//g' $tmp_dir/Exp3_update.csv >>$tmp_dir/Exp3_update_out.csv

cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_sample_out.csv >>$tmp_dir/Exp3_sample_done.csv
cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_update_out.csv >>$tmp_dir/Exp3_update_done.csv
cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_combined.csv >>$tmp_dir/Exp3_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/Exp3_sample_done.csv $destination_dir/lad_sample.png
python3 plotting/benchmark.py $tmp_dir/Exp3_update_done.csv $destination_dir/lad_update.png
python3 plotting/benchmark.py $tmp_dir/Exp3_combined_done.csv $destination_dir/lad_combined.png

# Clean
rm $tmp_dir/*.csv


echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/Exp3_header.csv

# Heap
grep 'Exp3,' out.csv >>$tmp_dir/Exp3_combined.csv
grep 'Exp3 (sample)' out.csv >>$tmp_dir/Exp3_sample.csv
grep 'Exp3 (update)' out.csv >>$tmp_dir/Exp3_update.csv

grep 'Exp3 Heap,' out.csv >>$tmp_dir/Exp3_combined.csv
grep 'Exp3 Heap (sample)' out.csv >>$tmp_dir/Exp3_sample.csv
grep 'Exp3 Heap (update)' out.csv >>$tmp_dir/Exp3_update.csv

sed 's/ (sample)//g' $tmp_dir/Exp3_sample.csv >>$tmp_dir/Exp3_sample_out.csv
sed 's/ (update)//g' $tmp_dir/Exp3_update.csv >>$tmp_dir/Exp3_update_out.csv

cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_sample_out.csv >>$tmp_dir/Exp3_sample_done.csv
cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_update_out.csv >>$tmp_dir/Exp3_update_done.csv
cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_combined.csv >>$tmp_dir/Exp3_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/Exp3_sample_done.csv $destination_dir/heap_sample.png
python3 plotting/benchmark.py $tmp_dir/Exp3_update_done.csv $destination_dir/heap_update.png
python3 plotting/benchmark.py $tmp_dir/Exp3_combined_done.csv $destination_dir/heap_combined.png

# Clean
rm $tmp_dir/*.csv


# Clean
rm $tmp_dir/*.csv


echo "name,k,iterations,real_time,cpu_time,time_unit,bytes_per_second,items_per_second,label,error_occurred,error_message" >>$tmp_dir/Exp3_header.csv

# Heap
grep 'Exp31,' out.csv >>$tmp_dir/Exp3_combined.csv
grep 'Exp31 (sample)' out.csv >>$tmp_dir/Exp3_sample.csv
grep 'Exp31 (update)' out.csv >>$tmp_dir/Exp3_update.csv

grep 'Exp31 Optimized,' out.csv >>$tmp_dir/Exp3_combined.csv
grep 'Exp31 Optimized (sample)' out.csv >>$tmp_dir/Exp3_sample.csv
grep 'Exp31 Optimized (update)' out.csv >>$tmp_dir/Exp3_update.csv

sed 's/ (sample)//g' $tmp_dir/Exp3_sample.csv >>$tmp_dir/Exp3_sample_out.csv
sed 's/ (update)//g' $tmp_dir/Exp3_update.csv >>$tmp_dir/Exp3_update_out.csv

cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_sample_out.csv >>$tmp_dir/Exp3_sample_done.csv
cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_update_out.csv >>$tmp_dir/Exp3_update_done.csv
cat $tmp_dir/Exp3_header.csv $tmp_dir/Exp3_combined.csv >>$tmp_dir/Exp3_combined_done.csv

python3 plotting/benchmark.py $tmp_dir/Exp3_sample_done.csv $destination_dir/exp31optimized_sample.png
python3 plotting/benchmark.py $tmp_dir/Exp3_update_done.csv $destination_dir/exp31optimized_update.png
python3 plotting/benchmark.py $tmp_dir/Exp3_combined_done.csv $destination_dir/exp31optimized_combined.png

# Clean
rm $tmp_dir/*.csv

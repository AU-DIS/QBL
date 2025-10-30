
alias python3="/usr/bin/python3"
#make_dir=cmake-build-servername

tmp_dir=/mnt/ssd/bandits
#tmp_dir=../tmp
make_dir=cmake-build-heroicis

name=adversarial

rounds=100000
#rounds=10000
averages=25
delta=0.9

run_experiment() {
      header=$tmp_dir/header_$name$1

      out=$tmp_dir/$name$1.out
      plt_out=$tmp_dir/$name$1.png

      out_mod2=$tmp_dir/$name$1_mod2.out
      plt_out_mod2=$tmp_dir/$name$1_mod2.png


      rm $header $out $plt_out 2> /dev/null


      echo "runner,dataset,gap,k,rounds,averages,delta,output_path" >> $header
      echo "adversarial,stochastically_constrained_adversarial,3.2,$1,$rounds,$averages,$delta,$out" >> $header
      echo "adversarial,mod2,3.2,$1,$rounds,$averages,$delta,$out_mod2" >> $header


      ./$make_dir/efficient_multi_armed_bandits $header
      python3 plotting/plot_adversarial.py $out $plt_out
      python3 plotting/plot_adversarial.py $out_mod2 $plt_out_mod2

}

for k in 4 8 16 32 128 256 512
do
    run_experiment $k
done
zip $tmp_dir/$name.zip $tmp_dir/*.out
rm $tmp_dir/*header* 2> /dev/null
rm $tmp_dir/*.out* 2> /dev/null
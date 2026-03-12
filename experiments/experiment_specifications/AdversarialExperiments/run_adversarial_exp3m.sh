
alias python3="$(which python3)"
#"/usr/bin/python3"

tmp_dir=/tmp
make_dir=.

name=exp3m

rounds=1000 #Set to 100000 for full experiments
averages=10
gap=3.2
delta=0.9
run_experiment() {
  header=$tmp_dir/header_$name$1m$2
  echo "run started"

  out=$tmp_dir/$name$1m$2.out
  plt_out=$tmp_dir/$name$1m$2.png

  out_mod2=$tmp_dir/$name$1m$2_mod2.out
  plt_out_mod2=$tmp_dir/$name$1m$2_mod2.png

  out_stochastic=$tmp_dir/$name$1m$2_stochastic.out
  plt_out_stochastic=$tmp_dir/$name$1m$2_stochastic.png

  out_tent=$tmp_dir/$name$1m$2_tent.out
  plt_out_tent=$tmp_dir/$name$1m$2_tent.png


  rm -f $header $out $plt_out $out_mod2 $plt_out_mod2 $out_stochastic $plt_out_stochastic $out_tent $plt_out_tent 2>/dev/null

  echo "runner,dataset,gap,k,m,rounds,averages,delta,output_path" >> $header
  echo "adversarial_exp3m,stochastically_constrained_adversarial,$gap,$1,$2,$rounds,$averages,$delta,$out" >>$header
  echo "adversarial_exp3m,mod2,$gap,$1,$2,$rounds,$averages,$delta,$out_mod2" >>$header
  echo "adversarial_exp3m,TentMapDataset,$gap,$1,$2,$rounds,$averages,$delta,$out_tent" >>$header
  (
    ./$make_dir/efficient_multi_armed_bandits $header &&
      python3 plotting/plot_compare.py $out $plt_out
      python3 plotting/plot_compare.py $out_mod2 $plt_out_mod2
      python3 plotting/plot_compare.py $out_tent $plt_out_tent
  ) &
  disown

}

for k in 20; do
  for m in 2 15; do
    run_experiment $k $m
  done
done
for k in 20; do
  for m in 20 150; do
    run_experiment $k $m
  done
done

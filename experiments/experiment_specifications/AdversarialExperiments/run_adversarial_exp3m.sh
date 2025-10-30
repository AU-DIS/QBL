
alias python3="$(which python3)"
#"/usr/bin/python3"

tmp_dir=/tmp
make_dir=.

if [[ $make_dir == cmake-build-release-djlama ]]; then
  mkdir -p /home/$(whoami)/tmp
  tmp_dir=/home/$(whoami)/tmp
fi

name=exp3m

rounds=100000
#rounds=10000
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

  out_duelling=$tmp_dir/$name$1m$2_duelling.out
  plt_out_duelling=$tmp_dir/$name$1m$2_duelling.png

  rm -f $header $out $plt_out $out_mod2 $plt_out_mod2 $out_stochastic $plt_out_stochastic $out_tent $plt_out_tent 2>/dev/null

  echo "runner,dataset,gap,k,m,rounds,averages,delta,output_path" >> $header
  echo "adversarial_exp3m,stochastically_constrained_adversarial,$gap,$1,$2,$rounds,$averages,$delta,$out" >>$header
  echo "adversarial_exp3m,mod2,$gap,$1,$2,$rounds,$averages,$delta,$out_mod2" >>$header
  echo "adversarial_exp3m,TentMapDataset,$gap,$1,$2,$rounds,$averages,$delta,$out_tent" >>$header
  #echo "adversarial_exp3m,DuellingDataset,$gap,$1,$2,$rounds,$averages,$delta,$out_duelling" >>$header

  (
    ./$make_dir/efficient_multi_armed_bandits $header &&
      python3 plotting/plot_compare.py $out $plt_out
      python3 plotting/plot_compare.py $out_mod2 $plt_out_mod2
      python3 plotting/plot_compare.py $out_tent $plt_out_tent
      #python3 plotting/plot_compare.py $out_duelling $plt_out_duelling
  ) &
  disown

}

for k in 20; do
  #for k in 2000; do
  #for m in 1800; do
  for m in 2 15; do
    run_experiment $k $m
  done
done
#zip $tmp_dir/$name.zip $tmp_dir/*.out

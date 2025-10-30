
#alias python3="/opt/homebrew/bin/python3"
alias python3="$(which python3)"
#"/usr/bin/python3"


make_dir=cmake-build-release-wsl
tmp_dir=/tmp
if [[ $make_dir == cmake-build-release-odin ]]; then
  mkdir -p /home/$(whoami)/tmp
  tmp_dir=/home/$(whoami)/tmp
fi

name=adversarial

run_experiment() {
    n=10
    m=$1
    averages=500
    for ((j=1, pow=n; j<m; j++)); do ((pow *= n)); done

    for K in 10 100 100
    do
      header=$tmp_dir/header_$name$1_$K
      out=$tmp_dir/$name$1.out_$K
      plt_out=$tmp_dir/$name$1_$K.png

      rm -f $header $out $plt_out 2> /dev/null

      echo "runner,K,rounds,averages,dataset,output_path" >> $header
      echo "GenericBanditRunner,$K,$pow,$averages,$name,$out" >> $header
      ./$make_dir/efficient_multi_armed_bandits $header
      python3 plotting/plot_regret.py $out $plt_out
    done
    rm $tmp_dir/*header* 2> /dev/null
    rm $tmp_dir/*.out* 2> /dev/null
}
for i in 1 #2 3 4 5
do
  run_experiment $i & disown
done
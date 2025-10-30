alias python3="$(which python3)"
make_dir=.


tmp_dir=/tmp

if [[ $make_dir == cmake-build-release-djlama ]]; then
  mkdir -p /home/$(whoami)/tmp
  tmp_dir=/home/$(whoami)/tmp
fi



name=QBL

rounds=100000
#rounds=10000
averages=50
gap=3.2
delta=0.9

run_experiment() {
    header=$tmp_dir/header_$name$1

    out=$tmp_dir/$name$1.out
    plt_out=$tmp_dir/$name$1.png

    out_mod2=$tmp_dir/$name$1_mod2.out
    plt_out_mod2=$tmp_dir/$name$1_mod2.png

    out_stochastic=$tmp_dir/$name$1_stochastic.out
    plt_out_stochastic=$tmp_dir/$name$1_stochastic.png

    out_tent=$tmp_dir/$name$1_tent.out
    plt_out_tent=$tmp_dir/$name$1_tent.png

    out_duelling=$tmp_dir/$name$1_duelling.out
    plt_out_duelling=$tmp_dir/$name$1_duelling.png

    rm $header $out $plt_out $out_mod2 $plt_out_mod2 $out_stochastic $plt_out_stochastic $out_tent $plt_out_tent 2>/dev/null

    echo "runner,dataset,gap,k,rounds,averages,delta,output_path,optimal_probability,optimal_proportion" >>$header
    echo "fpl_adversarial,stochastically_constrained_adversarial,$gap,$1,$rounds,$averages,$delta,$out,0.8,0.3" >>$header
    echo "fpl_adversarial,mod2,$gap,$1,$rounds,$averages,$delta,$out_mod2,0.9,0.2" >>$header
    echo "fpl_adversarial,TentMapDataset,$gap,$1,$rounds,$averages,$delta,$out_tent,0.9,0.2" >>$header
    #echo "fpl_adversarial,DuellingDataset,$gap,$1,$rounds,$averages,$delta,$out_duelling,0.9,0.2" >>$header
    (
        ./$make_dir/efficient_multi_armed_bandits $header &&
            python3 plotting/plot_compare.py $out $plt_out
            python3 plotting/plot_compare.py $out_mod2 $plt_out_mod2
            python3 plotting/plot_compare.py $out_tent $plt_out_tent
            #python3 plotting/plot_compare.py $out_duelling $plt_out_duelling
    ) &
    disown

}

for k in 20 100; do
    run_experiment $k
done
#zip $tmp_dir/$name.zip $tmp_dir/*$name*.out

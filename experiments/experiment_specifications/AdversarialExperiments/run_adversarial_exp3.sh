alias python3="$(which python3)"

tmp_dir=/mnt/ssd/bandits
make_dir=cmake-build-heroicis

name=exp3

rounds=100000
#rounds=10000
averages=1
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

    #out_duelling=$tmp_dir/$name$1_duelling.out
    #plt_out_duelling=$tmp_dir/$name$1_duelling.png

    rm $header $out $plt_out $out_mod2 $plt_out_mod2 $out_stochastic $plt_out_stochastic $out_tent $plt_out_tent 2>/dev/null

    echo "runner,dataset,gap,k,rounds,averages,delta,output_path" >>$header
    echo "exp3_adversarial,stochastically_constrained_adversarial,3.2,$1,$rounds,$averages,$delta,$out" >>$header
    echo "exp3_adversarial,mod2,3.2,$1,$rounds,$averages,$delta,$out_mod2" >>$header
    echo "exp3_adversarial,TentMapDataset,3.2,$1,$rounds,$averages,$delta,$out_tent" >>$header
    #echo "exp3_adversarial,DuellingDataset,3.2,$1,$rounds,$averages,$delta,$out_duelling" >>$header

    (
        ./$make_dir/efficient_multi_armed_bandits $header &&
        python3 plotting/plot_compare.py $out $plt_out && 
        python3 plotting/plot_compare.py $out_mod2 $plt_out_mod2 &&
        python3 plotting/plot_compare.py $out_tent $plt_out_tent
        #python3 plotting/plot_compare.py $out_duelling  $plt_out_duelling
    ) &
    disown

}

for k in 8 16 32; do
    run_experiment $k
done
#zip $tmp_dir/$name.zip $tmp_dir/$name*.out

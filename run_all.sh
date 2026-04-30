#!/bin/bash

if ! which root &>/dev/null ; then
    echo "No root command is available."
    echo "Probably you need execute 'source setup.csh'."
    exit
fi

N_MOM=10
N_ANG=10
for (( I_MOM = 0; I_MOM < N_MOM ; I_MOM++ )) ; do
for (( I_ANG = 0; I_ANG < N_ANG ; I_ANG++ )) ; do
for EXY in ex ex2 ey ; do
    LABEL=$(printf "sigma_${EXY}_p%02i_th%02i" $I_MOM $I_ANG)
    FN_IN="infiles/$LABEL.inp"
    test -e $FN_IN || continue
    echo "================================================================"
    echo "FN_IN = $FN_IN"
    ./run_mc_single_arm $LABEL
    rm worksim/$LABEL.rzdat
    
    test $EXY = ey && EXY_TYPE=Ey || EXY_TYPE=Ex
    root -b -l -q "format_tree.cc(\"$LABEL\", \"$EXY_TYPE\")"
    rm worksim/$LABEL.root
    
    root -b -l -q "analyze_tree.cc(\"$LABEL\")"
    root -b -l -q "eval_stat.cc(\"$LABEL\")"
done
done
done

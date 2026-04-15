#!/bin/bash

N_MOM=10
N_ANG=10
for (( I_MOM = 0; I_MOM < N_MOM ; I_MOM++ )) ; do
for (( I_ANG = 0; I_ANG < N_ANG ; I_ANG++ )) ; do
for EXY in ex ey ; do
    LABEL=$(printf "sigma_${EXY}_p%02i_th%02i" $I_MOM $I_ANG)
    FN_IN="infiles/$LABEL.inp"
    test -e $FN_IN || continue
    echo "================================================================"
    echo "FN_IN = $FN_IN"
    ./run_mc_single_arm $LABEL

    # You have to call 'source setup.csh' in advance
    root -b -l -q "analyze_tree.cc(\"$LABEL\")"
done
done
done

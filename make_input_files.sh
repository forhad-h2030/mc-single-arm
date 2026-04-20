#!/bin/bash

LIST_MOM=( 3000 5000 7500 ) # MeV
LIST_ANG=( 6.5 25.0 39.0 ) # deg
N_MOM=${#LIST_MOM[*]}
N_ANG=${#LIST_ANG[*]}
echo "N_MOM, N_ANG = $N_MOM, $N_ANG"

for EXY in ex ex2 ey ; do
    FN_BASE=infiles/sigma_${EXY}.inp.template
    echo "FN_BASE = $FN_BASE"
    test -e $FN_BASE || continue
    for (( I_MOM = 0 ; I_MOM < $N_MOM ; I_MOM++ )) ; do
    for (( I_ANG = 0 ; I_ANG < $N_ANG ; I_ANG++ )) ; do
	MOM=${LIST_MOM[$I_MOM]}
	ANG=${LIST_ANG[$I_ANG]}
	FN_OUT=$(printf "infiles/sigma_${EXY}_p%02i_th%02i.inp" $I_MOM $I_ANG)
	echo "  I($I_MOM, $I_ANG) ($MOM, $ANG) --- $FN_OUT"
	sed -e "s/_MOM_SP_/$MOM/" -e "s/_ANG_SP_/$ANG/" $FN_BASE >$FN_OUT
    done
    done
done

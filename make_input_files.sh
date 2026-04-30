#!/bin/bash

echo "Remove existing inp files."
rm -f infiles/sigma_e*_p*_th*.inp

#NUM_EV=100000
NUM_EV=1000000
echo "NUM_EV = $NUM_EV"

#LIST_MOM=( 3000 5000 7500 ) # MeV
#LIST_ANG=( 6.5 25.0 39.0 ) # deg
#LIST_MOM=( 3000 4000 5000 ) # MeV
#LIST_ANG=( 10.0 15.0 20.0 ) # deg
#LIST_MOM=( 5000 ) # MeV
#LIST_ANG=( 10.0 ) # deg
#LIST_MOM=( 7500 ) # MeV
#LIST_ANG=( 8.0 10.0 12.0 14.0 ) # deg
#LIST_MOM=( 7500 ) # MeV
#LIST_ANG=( 8.0 10.0 12.0 14.0 ) # deg
#LIST_MOM=( 7500 ) # MeV
#LIST_ANG=( 8.0 ) # deg
LIST_MOM=( 3000 ) # MeV
LIST_ANG=( 8.0 10.0 12.0 14.0 ) # deg
N_MOM=${#LIST_MOM[*]}
N_ANG=${#LIST_ANG[*]}
echo "N_MOM, N_ANG = $N_MOM, $N_ANG"

#ex ex2 ey
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
	sed -e "s/_NUM_EV_/$NUM_EV/" -e "s/_MOM_SP_/$MOM/" -e "s/_ANG_SP_/$ANG/" $FN_BASE >$FN_OUT
    done
    done
done

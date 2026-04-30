#!/bin/bash
# Script to run the MC multiple times (i.e. jobs).
# I found this necessary because the MC failed
# when the number of MC trials was 10M.

if ! which root &>/dev/null ; then
    echo "No root command is available."
    echo "Probably you need execute 'source setup.csh'."
    exit
fi

JOB_B=0
JOB_E=20
echo "JOB_B...E = $JOB_B...$JOB_E"

I_MOM=0
I_ANG=0
for EXY in ex ey ; do
    LABEL=$(printf "sigma_${EXY}_p%02i_th%02i" $I_MOM $I_ANG)
    FN_IN="infiles/$LABEL.inp"
    test -e $FN_IN || continue
    echo "================================================================"
    echo "FN_IN = $FN_IN"
    for (( I_JOB = $JOB_B ; I_JOB < $JOB_E ; I_JOB++ )) ; do
	echo "----------------"
	echo "I_JOB = $I_JOB"
	./run_mc_single_arm $LABEL
	
	LABEL_JOB=$(printf "%s_j%03i" $LABEL $I_JOB)
	mv worksim/$LABEL.root worksim/$LABEL_JOB.root

	test $EXY = ey && EXY_TYPE=Ey || EXY_TYPE=Ex
	root -b -l -q "format_tree.cc(\"$LABEL_JOB\", \"$EXY_TYPE\")"
	rm worksim/$LABEL_JOB.root

	#root -b -l -q "analyze_tree.cc(\"$LABEL\")"
	#root -b -l -q "eval_stat.cc(\"$LABEL\")"
    done
done

# Usage of Modified Version

This document explains the usage of this modified version being developed by UVA in April 2026.
The contents of README.md are mostly valid, but some practical factors are different.

Branch `target-field` must be always used.


## Build

```
cd /path/to/mc-single-arm/src
make
```

Note that `module use ...` etc. are not necessary at ifarm.


## Input Files

Multiple input files can be created by `make_input_files.sh`:
```
cd /path/to/mc-single-arm
./make_input_files.sh
```

The script reads template files
* sigma_ex.inp.template
* sigma_ex2.inp.template
* sigma_ey.inp.template
to create usable input files like `sigma_ex_p00_th00.inp`,
where `p00` and `th00` mean an index of spectrometer setting.
You can edit the script to change/increase/decrease the settings to be created.


## Execution

A script (`run_all.sh`) carries out all steps with all input files created above.

```
cd /path/to/mc-single-arm
source setup.csh
./run_all.sh
```

The script loops over all the input files.
Per input file, it calls three programs;
* `run_mc_single_arm` to carry out event generation,
* `format_tree.cc` to reformat the generated file, and
* `analyze_tree.cc` to analyze the events.
Please look into the three programs to check their functions.

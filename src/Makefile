## JLab-compatible Makefile for mc-single-arm (target-field)

RM        = rm -f
SHELL     = /bin/sh

H      = hms/
SH     = shms/
SHARE  = shared/

my_objs = hms_hbook_init.o shms_hbook_init.o \
          target_cans.o trg_track.o \
          $(H)mc_hms.o $(H)mc_hms_hut.o $(H)mc_hms_recon.o \
          $(SH)mc_shms.o $(SH)mc_shms_hut.o $(SH)mc_shms_recon.o \
          $(SHARE)project.o $(SHARE)transp.o $(SHARE)rotate_haxis.o \
          $(SHARE)rotate_vaxis.o $(SHARE)locforunt.o $(SHARE)musc.o \
          $(SHARE)musc_ext.o $(SHARE)stringlib.o mt19937.o gauss1.o \
          loren.o

MYOS := $(subst -,,$(shell uname))

# Linux (JLab EL9) configuration

ifeq ($(MYOS),Linux)

  CERN_ROOT = /cvmfs/oasis.opensciencegrid.org/jlab/scicomp/sw/el9/cernlib/2023

  FC  := gfortran
  F77 := gfortran

  FFLAGS = -O -ffixed-line-length-132 -ff2c -fno-automatic -fdefault-real-8
  FFLAG1 = $(FFLAGS) -c

CERNLIBS = -L$(CERN_ROOT)/lib \
           -lpacklib -lmathlib -lkernlib -lpawlib -lgraflib -lgrafX11

  OTHERLIBS = $(CERNLIBS) -lX11 -lm

endif

%.o: %.f
	$(F77) $(FFLAGS) -c $< -o $@

all: mc_single_arm

mc_single_arm: $(my_objs) mc_single_arm.o
	$(F77) -o $@ $(FFLAGS) mc_single_arm.o $(my_objs) $(OTHERLIBS)

clean:
	$(RM) *.o $(H)*.o $(SH)*.o $(SHARE)*.o mc_single_arm

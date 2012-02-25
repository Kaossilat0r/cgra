# app specific

CFLAGS=-I$(DRESC_PATH)/include
LDFLAGS=-L$(DRESC_PATH)/lib -lshared_intrinsics

ARGS= > output/result

PRE_RUN=rm -rf output && mkdir -p output
CLEAN_RUN=rm -rf output

PRE_REF=rm -rf ref_output output && mkdir -p output
POST_REF=mv -f output ref_output

COMPARE=diff -q output/result ref_output/result

# common
include $(DRESC_PATH)/share/appbuild/Makefile.app


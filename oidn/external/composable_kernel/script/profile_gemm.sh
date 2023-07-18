#!/bin/bash

## GPU visibility
export HIP_VISIBLE_DEVICES=0
DRIVER="../build/bin/ckProfiler"
echo $DRIVER
OP=$1
DATATYPE=$2
LAYOUT=$3
VERIFY=$4
INIT=$5
LOG=$6
TIME=$7


# 120 CU
########  op  datatype  layout  verify  init  log  time  M___ N___ K___  StrideA StrideB StrideC
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  960  1024 1024       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  960  2048 2048       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 1920  1024 2048       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 1920  2048 2048       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 3840  4096 4096       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 7680  8192 8192       -1     -1      -1
 
# 104 CU
########  op  datatype  layout  verify  init  log  time  M___ N___ K___  StrideA StrideB StrideC
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  832  1024 1024       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  832  2048 2048       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 1664  1024 2048       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 1664  2048 2048       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 3328  4096 4096       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 6656  8192 8192       -1     -1      -1
 
# 110 CU
########  op  datatype  layout  verify  init  log  time  M___ N___ K___  StrideA StrideB StrideC
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 1280  1408 1024       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 1280  2816 2048       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 2560  1408 2048       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 2560  2816 2048       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 5120  5632 4096       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 7040  8192 8192       -1     -1      -1

# testing different strides
########  op  datatype  layout  verify  init  log  time  M___ N___ K___  StrideA StrideB StrideC
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 1024  1024 1024	    1024   1024    1024
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 2048  2048 2048	    2048   2048    2048
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 4096  4096 4096	    4096   4096    4096
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 8192  8192 8192	    8192   8192    8192
 
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 1024  1024 1024	    1056   1056    1056
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 2048  2048 2048	    2080   2080    2080
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 4096  4096 4096	    4128   4128    4128
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 8192  8192 8192	    8224   8224    8224
 
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 1024  1024 1024	    1088   1088    1088
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 2048  2048 2048	    2112   2112    2112
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 4096  4096 4096	    4160   4160    4160
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME 8192  8192 8192	    8256   8256    8256

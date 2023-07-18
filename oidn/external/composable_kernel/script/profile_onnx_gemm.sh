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
# GEMM kernel benchmarks used by ONNX 
########  op  datatype  layout  verify  init  log  time  M___ N___ K___  StrideA StrideB StrideC
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  384  768  768        -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  384  768  2304       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  384  768  3072       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  384  3072 768        -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  384  1024 1024       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  384  1024 3072       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  384  1024 4096       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  384  4096 1024       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  24576 768 768        -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  24576 768 2304       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  24576 768 3072       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  24576 3072 768       -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  24576 1024 1024      -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  24576 1024 3072      -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  24576 1024 4096      -1     -1      -1
 $DRIVER $OP $DATATYPE $LAYOUT $VERIFY $INIT $LOG $TIME  24576 4096 1024      -1     -1      -1
 

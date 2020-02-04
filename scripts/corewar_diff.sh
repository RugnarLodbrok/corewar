#!/bin/bash

OG_COREWAR="./asm/vm_champs/corewar"
MY_COREWAR="./corewar"
CHAMPS="champs/marcher.cor"
MODE=$1
STEPS=$2
shift
#MODE_ALL=27

if [ "$MODE" == 0 ]
then
  ${MY_COREWAR} -d $STEPS $CHAMPS > tmp/a.txt
  ${OG_COREWAR} -d $STEPS $CHAMPS > tmp/b.txt
  diff tmp/a.txt tmp/b.txt
else
  ${MY_COREWAR} -v 31 -d $STEPS $CHAMPS | grep -v -E "0x(\d|\w)+ : " > tmp/a.txt
  ${OG_COREWAR} -v 31 -d $STEPS $CHAMPS | sed "s///" | grep -v -E "0x(\d|\w)+ : " > tmp/b.txt
  diff tmp/a.txt tmp/b.txt
#  echo "---"
#  cat tmp/a.txt | grep -A1 -B1 Cycle > tmp/a1.txt
#  cat tmp/b.txt | grep -A1 -B1 Cycle > tmp/b1.txt
#  cat tmp/a1.txt tmp/b1.txt
fi

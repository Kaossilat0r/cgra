#!/bin/bash
# $1: stat.out
# $2: dresc_arch

DRESC_HOME="/opt/dresc"
USED_CLOCK=`head -n 2 $1 | grep 'Total' | cut -d ' ' -f 4`
USED_INSTANCE=`head -n 4 $2 | grep 'instance:' | cut -d ' ' -f 5`
INSTANCE_RAW_NUM=`echo $USED_INSTANCE | cut -d 'x' -f 1`
INSTANCE_COL_NUM=`echo $USED_INSTANCE | cut -d 'x' -f 2`
FU_NUM=$(($INSTANCE_RAW_NUM*$INSTANCE_COL_NUM))
NOP_NUM=$(($FU_NUM*$USED_CLOCK))
ALL_USED_INS=`head -n 101 $1 | grep 'Total.*\.ins = [1-9]' | cut -d ' ' -f 2,4 | sed 's/.ins /:/g'`
USED_ENERGY="0.0"
# sleep 2
echo -e "\n\n\n**************\n** statistics **\n**************"
printf '%-15s\t %+15s\t %+10s\t %+15s\t %+15s\t\n' "Instr" "Num" "Clk/Instr" "Energy/Clk" "Total energy"
echo -e "------------------------------------------------------------------------------------------------"

for INS in $ALL_USED_INS
do
	CURRENT_INS=`echo $INS | cut -d ':' -f 1`
#	echo -n "$CURRENT_INS   "
	CURRENT_INS_CNT=`echo $INS | cut -d ':' -f 2`
	CURRENT_INS_ENERGY=`cat $DRESC_HOME/share/adres_energy_statistic | grep "^${CURRENT_INS}:" | cut -d ':' -f 2 |  cut -d ' ' -f 1`
	CURRENT_INS_DELAY=`cat $DRESC_HOME/share/adres_energy_statistic | grep "^${CURRENT_INS}:" | cut -d ' ' -f 2`
	if [ "$CURRENT_INS_ENERGY" == "" ];then
		echo "$CURRENT_INS existiert nicht!"
		CURRENT_INS_ENERGY="1.0"
		CURRENT_INS_DELAY="1"
	fi
#	echo "$CURRENT_INS(energy=$CURRENT_INS_ENERGY,delay=$CURRENT_INS_DELAY) wird $CURRENT_INS_CNT mal ausgeführt!"
#	echo -n "$CURRENT_INS verbraucht "
#	echo "scale=11;$CURRENT_INS_ENERGY*$CURRENT_INS_CNT*$CURRENT_INS_DELAY" | bc -q
	USED_ENERGY=`echo "scale=11;$CURRENT_INS_ENERGY*$CURRENT_INS_CNT*$CURRENT_INS_DELAY+$USED_ENERGY" | bc -q 2>/dev/null`		 
	printf '%-15s\t %+15s\t %+10s\t %+15s\t %+15s\t\n' "$CURRENT_INS" "$CURRENT_INS_CNT" "$CURRENT_INS_DELAY" "$CURRENT_INS_ENERGY" "`echo "scale=11;$CURRENT_INS_ENERGY*$CURRENT_INS_CNT*$CURRENT_INS_DELAY" | bc -q`"
	CURRENT_INS_CLK_NUM=$(($CURRENT_INS_CNT*$CURRENT_INS_DELAY))
#	echo  "CURRENT_INS_CLK_NUM:$CURRENT_INS_CNT x $CURRENT_INS_DELAY = $CURRENT_INS_CLK_NUM"
#	echo -n "NOP:$NOP_NUM-$CURRENT_INS_CLK_NUM="
	NOP_NUM=$(($NOP_NUM-$CURRENT_INS_CLK_NUM))
#	echo "$NOP_NUM"
done
printf '%-15s\t %+15s\t %+10s\t %+15s\t %+15s\t\n' "nop" "$NOP_NUM" "1" "0.3" "`echo "scale=11;0.3*$NOP_NUM" | bc -q`"
# echo  "USED_ENERGY=$USED_ENERGY"
USED_ENERGY=`echo "scale=11;0.3*$NOP_NUM+$USED_ENERGY" | bc -q 2>/dev/null`
# echo  "USED_ENERGY=$USED_ENERGY"

# echo -e "*****************"
# echo "Total FUs: $FU_NUM"
# echo "Total NOP: $NOP_NUM"


echo -e "\n\n\n**************\n** overview **\n**************"
# echo -e "\n\n\n\n** overview **"
echo "Used instance : $USED_INSTANCE"
echo "Total clocks on cga : `head -n 3 $1 | grep "Total cga" | cut -d ' ' -f 5`"
echo "Total clocks  : $USED_CLOCK"
echo "Total nop-clk : $NOP_NUM "
echo "Total energy  : $USED_ENERGY"

echo -e "********************************"
echo -e "********************************"

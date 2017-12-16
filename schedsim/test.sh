#!/bin/bash

#CONSTS
EXECUTABLE=./schedsim;
GANTT_GPLOT=../gantt-gplot
RESULTS=resultados;
EXEC_GPLOT=generate_gantt_chart


flag=false;
inputFile="";

while [ "$flag" != "true" ]; do
	read -p "Please, give me a file to simulate: " inputFile;
	
	if [ -f $inputFile ]; then
		flag=true;
	else
		echo "$inputFile is not a valid file";
	fi
	
done;

flag=false;
maxCPUs=0;

while [ $flag != "true" ]; do
	read -p "How many CPU's do you want to use in the simulation?" maxCPUs;
	
	if (( $maxCPUs >= 1 && $maxCPUs <= 8 )); then
		flag=true;
	else
		echo "The number of Cpu's introduced is not valid";
	fi
done;

if [ ! -x $EXECUTABLE ]; then
	echo "It was not possible to find the simulator in the current directory";
	exit -1;
fi

if [ -d $RESULTS ]; then
	echo "Removing the directory $RESULTS";
	rm -rf $RESULTS
fi;

mkdir $RESULTS

curDir=$(pwd)

for algorithmScheduler in $($EXECUTABLE -L | cut -d : -f 2); do
	for i in $(seq 1 $maxCPUs); do 
		echo "Simulating with algorithm: $algorithmScheduler and $i CPU's";
		./$EXECUTABLE -i $inputFile -n $i -s $algorithmScheduler;
		for fileLog in *.log; do
			mv $fileLog ./$RESULTS/$algorithmScheduler"_CPUs_"$i"_"$fileLog
			cd $GANTT_GPLOT
			./$EXEC_GPLOT $curDir/$RESULTS/$algorithmScheduler"_CPUs_"$i"_"$fileLog
			cd $curDir
		done;
		echo "----------";
	done;
done;

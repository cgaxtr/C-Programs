#!/bin/bash

#colors for bash shell
RED_INIT="\e[1;31m"
RED_FINISH="\e[0m"
GREEN_INIT="\e[1;32m"
GREEN_FINISH="\e[0m"

#paths

TMP="./tmp"
MOUNT="./mount-point"

FILE1="./src/fuseLib.c"
FILE2="./src/myFS.h"
FILE3="./src/MyFileSystem.c"

if [ -d $TMP ]; then
    echo "Cleaning temporal directory...";
    rm -r $TMP;
fi
mkdir $TMP;

#copy first file
nameFile1=$( echo $FILE1 | cut -d '/' -f 3)
echo "[COPYING] file: $nameFile1"
cp $FILE1 $TMP
cp $FILE1 $MOUNT

#copy second file
nameFile2=$( echo $FILE2 | cut -d '/' -f 3)
echo "[COPYING] file: $nameFile2"
cp $FILE2 $TMP
cp $FILE2 $MOUNT

#checking the integrity for our disk
./my-fsck-static-64 virtual-disk

#compare file1
if [ "$(diff $MOUNT/$nameFile1 $TMP/$nameFile1)" == "" ]; then
	echo -e "["$GREEN_INIT"EQUALS"$GREEN_FINISH"] The copy of $nameFile1 it's ok"
else
	echo -e "["$RED_INIT"DIFFERENT"$RED_FINISH"] The copy of $nameFile1 it's bad"
	exit -1;
fi;

#compare file2
if [ "$(diff $MOUNT/$nameFile2 $TMP/$nameFile2)" == "" ]; then
	echo -e "["$GREEN_INIT"EQUALS"$GREEN_FINISH"] The copy of $nameFile2 it's ok"
else
	echo -e "["$RED_INIT"DIFFERENT"$RED_FINISH"] The copy of $nameFile2 it's bad"
	exit -1;
fi;

#truncate file1 
#BLOCK SIZE IS 4096
echo "[TRUNCATE] removing one block to $nameFile2"
truncate --size=-4096 $TMP/$nameFile1
truncate --size=-4096 $MOUNT/$nameFile1

#checking the integrity for our disk
./my-fsck-static-64 virtual-disk

#compare file1(truncate)
if [ "$(diff $MOUNT/$nameFile1 $TMP/$nameFile1)" == "" ]; then
	echo -e "["$GREEN_INIT"EQUALS"$GREEN_FINISH"] The truncate of $nameFile1 it's ok"
else
	echo -e "["$RED_INIT"DIFFERENT"$RED_FINISH"] The truncate of $nameFile1 it's bad"
	exit -1;
fi;

#coopying other file
nameFile3=$( echo $FILE3 | cut -d '/' -f 3)
echo "[COPYING] file: $nameFile3"
cp $FILE3 $TMP
cp $FILE3 $MOUNT

#checking the integrity for our disk
./my-fsck-static-64 virtual-disk

#compare file3
if [ "$(diff $MOUNT/$nameFile3 $TMP/$nameFile3)" == "" ]; then
	echo -e "["$GREEN_INIT"EQUALS"$GREEN_FINISH"] The copy of $nameFile3 it's ok"
else
	echo -e "["$RED_INIT"DIFFERENT"$RED_FINISH"] The copy of $nameFile3 it's bad"
	exit -1;
fi;

echo "[TRUNCATE] adding one block to $nameFile2"
truncate --size=+4096 $TMP/$nameFile2
truncate --size=+4096 $MOUNT/$nameFile2

#checking the integrity for our disk
./my-fsck-static-64 virtual-disk

#compare file2 truncate
if [ "$(diff $MOUNT/$nameFile2 $TMP/$nameFile2)" == "" ]; then
	echo -e "["$GREEN_INIT"EQUALS"$GREEN_FINISH"] The truncate of $nameFile2 it's ok"
else
	echo -e "["$RED_INIT"DIFFERENT"$RED_FINISH"] The truncate of $nameFile2 it's bad"
	exit -1;
fi;

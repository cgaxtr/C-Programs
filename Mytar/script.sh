#!/bin/bash

if [ ! -e ./mytar ]; then
	echo "ERROR: El fichero no existe"
	exit -1
elif [ ! -x ./mytar ]; then
	echo "ERROR: el programa mytar no es ejecutable"
fi

if [ -d ./tmp ]; then
	rm -r ./tmp
fi

mkdir tmp
cd tmp

echo "Hello world!" > file1.txt
head /etc/passwd > file2.txt
head -c 1024 /dev/urandom > file3.dat

./../mytar -cf filetar.mtar file1.txt file2.txt file3.dat

mkdir out

cp filetar.mtar ./out

cd out

./../../mytar -xf filetar.mtar

if ! diff file1.txt ../file1.txt >> /dev/null; then
	echo "El fichero file1.txt no es correcto"
	exit -1
fi

if ! diff file2.txt ../file2.txt >> /dev/null; then
	echo "El fichero file2.txt no es correcto"
	exit -1
fi

if ! diff file3.dat ../file3.dat >> /dev/null; then
	echo "El fichero file3.dat no es correcto"
	exit -1
fi

echo "Comprobacion del programa completada con exito"

exit 0


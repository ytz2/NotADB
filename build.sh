#!/bin/bash

DIRECTORY=`dirname $0`
BUILDDIR=`realpath $DIRECTORY/build`

CURDIR=`pwd`

if [ -d "${BUILDDIR}" ] 
then
    echo "remove exiting build directory ${BUILDDIR}" 
    rm -rf $DIRECTORY/build	
fi

echo "creating build directory ${BUILDDIR}"
mkdir $BUILDDIR
cd $BUILDDIR
cmake ../src/
make
cd $CUDIR
echo "building finished"
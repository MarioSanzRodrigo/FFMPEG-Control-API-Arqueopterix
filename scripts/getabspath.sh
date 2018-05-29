#!/bin/bash

CURR_DIR=`pwd`
DIR_PATH=`cd $1;pwd`
cd $CURR_DIR
if [ -d $DIR_PATH ];
then
   echo "$DIR_PATH"
#else
#   echo "directory_"$DIR_PATH"_does_not_exists"
fi

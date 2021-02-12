#!/bin/bash

### Written By Alireza Zarenejad

# ./test.sh <directory_with_makefile>

# Defining colors
RED="\033[0;31m"
GREEN="\033[0;32m"
CYAN="\033[0;36m"
NC='\033[0m' # No Color
TEMP_DIR="bin"
TESTCASE_DIR="testcases"
ASSETS_DIR="test_assets"
EXECUTABLE_FILE="./StoreCalculator.out"
IN_POSTFIX="txt"
OUT_POSTFIX="out"

mkdir -p $TEMP_DIR
cp -r $1/* $TEMP_DIR
cp -r $ASSETS_DIR $TEMP_DIR
cp $TESTCASE_DIR/*.$IN_POSTFIX $TEMP_DIR
cd $TEMP_DIR
make clean

counter=0
numOfTestCases=$(ls -1q ../$TESTCASE_DIR/*.txt | wc -l)
for ((i = 1; i <= $numOfTestCases; i++))
do
	make
    if ! $EXECUTABLE_FILE $i.$IN_POSTFIX $ASSETS_DIR | diff -wB ../$TESTCASE_DIR/$i.$OUT_POSTFIX -; then
        echo -e "${RED}$i - Failed!${NC}"
    else
        echo -e "${GREEN}$i - Passed!${NC}"
        counter=$(( counter + 1 ));
    fi
    
    make clean

done

cd ..
rm -rf $TEMP_DIR
echo -e "\n${CYAN}######## $counter test cases passed! ########${NC}"

code $1

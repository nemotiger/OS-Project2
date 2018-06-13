#!/usr/bin/env bash

if [[ $# -ne 3 ]]; then
    echo "$0 <src file> <master method> <slave method>"
    echo "method = fcntl | mmap"
    exit
fi

FILE_IN=${1}
FILE_OUT=${1}.out
MASTER_METHOD=${2}
SLAVE_METHOD=${3}

RESULT=result
TMP=$(mktemp -u)

echo "----- $(date) -----" >> $RESULT
echo "src/user_program/master data/"$FILE_IN" "$MASTER_METHOD"" >> $RESULT
echo "src/user_program/slave data/"$FILE_OUT" "$SLAVE_METHOD" 127.0.0.1" >> $RESULT
echo "Master Device:" >> $RESULT

echo "***** Start Testing *****"
echo "src/user_program/master data/"$FILE_IN" "$MASTER_METHOD""
echo "src/user_program/slave data/"$FILE_OUT" "$SLAVE_METHOD" 127.0.0.1"

src/user_program/master data/"$FILE_IN" "$MASTER_METHOD" >>$RESULT 2>>$RESULT &
src/user_program/slave data/"$FILE_OUT" "$SLAVE_METHOD" 127.0.0.1 >>$TMP 2>>$TMP &

wait

echo "Writing test results to ./result"

echo "Slave Device:" >> $RESULT
cat "$TMP" >> $RESULT
echo "" >> $RESULT

echo "***** Test Finished *****"
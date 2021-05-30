#!/bin/sh

set -e

DIR="../iccap/examples/model_files"

echo > tested.txt

for FILE in `cat mdls_good.txt`
do
    echo _---------------------------------------------------
    echo $FILE
    echo _---------------------------------------------------
    echo "$FILE" >> tested.txt
    ./parse_file "$FILE" > out.txt
done

echo SUCCESS

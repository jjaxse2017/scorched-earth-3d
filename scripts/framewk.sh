#!/bin/bash

binary=$1

fwks=`otool -L $binary | awk '{if (/Library/ && !/System/) print $1; };'`;

for f in $fwks; do

org=$f
new=`echo $f | awk '{gsub(/\/Library/, "", $1); print "@executable_path/.." $1;}'`;

echo $org $new

install_name_tool -change $org $new $binary

done

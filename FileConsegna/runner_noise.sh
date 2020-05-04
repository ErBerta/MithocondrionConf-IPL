#!/bin/bash
var1="_"
var2=".bmp"
fn="flower2"
method="corrupt"

for amount in 10 20 50 100 150 200 255
do
    out=$fn$var1$method$var1$amount
    echo "$out"
    ./main $fn$var2 0 $method $out 0 $amount
done
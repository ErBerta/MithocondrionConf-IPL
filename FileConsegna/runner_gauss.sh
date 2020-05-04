#!/bin/bash
var1="_"
var2=".bmp"
fn="mongolfiere"
method="gauss"

for kernel_size in 3 5 7 9 11
do
  for sigma in 0.1 0.5 1 1.5 2 3 5
  do
      out=$fn$var1$method$var1$kernel_size$var1$sigma$var2
      echo "$out"
      ./main $fn$var2 0 $method $out 0 $kernel_size $sigma
  done
done
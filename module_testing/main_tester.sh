#!/bin/bash
# mutually exclusive methods:
#   corrupt, gray, brighten, blend, sharp, edge, emboss, avg, gauss
#
#
#
#
#

MAIN_EXEC=main
IMG_DIR=images/
IMG1="mongolfiere.bmp"
IMG2="flower2.bmp"
declare -a methods_no_kernel=("gray" "sharp" "edge" "emboss")
declare -a methods_yes_kernel=("corrupt" "brighten" "blend")

for i in "${methods_no_kernel[@]}"
do
  echo $i
  ./$MAIN_EXEC "$IMG_DIR$IMG1" "$IMG_DIR$IMG2" $i "$IMG_DIR$i.bmp" 0
done

i=0
out=""
# method: corrupt
for amount in 10 20 50 100 150 200 255
do
  out="$IMG_DIR${methods_yes_kernel[$i]}_$amount.bmp"
  echo $out
  ./$MAIN_EXEC "$IMG_DIR$IMG1" 0 ${methods_yes_kernel[$i]} $out 0
done

i=$i+1;
# method: brighten
for amount in 10 20 50 100 150 200 255
do
  out="$IMG_DIR${methods_yes_kernel[$i]}_$amount.bmp"
  echo $out
  ./$MAIN_EXEC "$IMG_DIR$IMG1" 0 ${methods_yes_kernel[$i]} $out 0
done

i=$i+1;
# method: blend
for amount in 0.0 0.25 0.50 0.75 1.0
do
  out="$IMG_DIR${methods_yes_kernel[$i]}_$amount.bmp"
  echo $out
  ./$MAIN_EXEC "$IMG_DIR$IMG1" "$IMG_DIR$IMG2" ${methods_yes_kernel[$i]} $out 0
done


./runner_gauss.sh
#!/bin/bash
shopt -s extglob
IMG_DIR=images/
cd $IMG_DIR
rm -v !("caf.bmp"|"emboss.bmp"|"flower.bmp"|"flower2"|"flower2_corrupt.bmp"|"fullmoon.bmp"|"mongolfiere.bmp")
cd ..
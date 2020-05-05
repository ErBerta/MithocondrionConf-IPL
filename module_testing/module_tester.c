#include <stdio.h>
#include <stdlib.h>
#include "ip_lib.h"
#include "bmp.h"

int main(){
	Bitmap* b = NULL;
	ip_mat* input_img = NULL;
    b = bm_load("mongolfiere.bmp");
	input_img = bitmap_to_ip_mat(b);
	ip_mat_show(input_img);
	free(b);
	b = NULL;
	free(input_img);
	input_img = NULL;
	return 0;
}

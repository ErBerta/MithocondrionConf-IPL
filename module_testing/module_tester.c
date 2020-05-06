#include <stdio.h>
#include <stdlib.h>
#include "ip_lib.h"
#include "bmp.h"

int main(){
	Bitmap* b = NULL;
	ip_mat* input_img = NULL;
	ip_mat* mod_ip_mat = NULL;
    b = bm_load("mongolfiere.bmp");
	input_img = bitmap_to_ip_mat(b);
	ip_mat_show(input_img);
	mod_ip_mat = ip_mat_mul_scalar(input_img, 0.1);
	ip_mat_show(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
	mod_ip_mat = NULL;
	mod_ip_mat = ip_mat_add_scalar(input_img, 0.99999);
	ip_mat_show(mod_ip_mat);

	bm_free(b);
	ip_mat_free(input_img);
	ip_mat_free(mod_ip_mat);

	return 0;
}

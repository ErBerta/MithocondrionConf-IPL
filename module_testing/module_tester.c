#include <stdio.h>
#include <stdlib.h>
#include "ip_lib.h"
#include "bmp.h"

int main(){
	Bitmap* b = NULL;
	Bitmap* c = NULL;
	ip_mat* input_img = NULL;
	ip_mat* input_img2 = NULL;

	/* "Lab Rat" ip_mat */
	ip_mat* mod_ip_mat = NULL;

    /* Load a bmp image */
    b = bm_load("mongolfiere.bmp");
	input_img = bitmap_to_ip_mat(b);
	ip_mat_show(input_img);

    /* Test a function, show on screen the output, free up the memory and set the pointer to NULL*/
	mod_ip_mat = ip_mat_mul_scalar(input_img, 0.1);
	ip_mat_show(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
	mod_ip_mat = NULL;

	mod_ip_mat = ip_mat_add_scalar(input_img, 0.99999);
	ip_mat_show(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
	mod_ip_mat = NULL;

    c = bm_load("mongolfiere.bmp");
	/* Necessario esegure cast a float? */
	input_img2 = bitmap_to_ip_mat(c);

	mod_ip_mat = ip_mat_sum(input_img,input_img2);
	ip_mat_show(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
	mod_ip_mat = NULL;

	mod_ip_mat = ip_mat_sub(input_img,input_img2);
	ip_mat_show(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
	mod_ip_mat = NULL;

	mod_ip_mat = ip_mat_mean(input_img,input_img2);
	ip_mat_show(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
    mod_ip_mat = NULL;

    /* Don't free mod_ip_mat in this section here, but rather do it after each call to a function*/
	bm_free(b);
	bm_free(c);
	ip_mat_free(input_img);
	ip_mat_free(input_img2);

	return 0;
}

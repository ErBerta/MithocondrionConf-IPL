#include <stdio.h>
#include <stdlib.h>
// #include <time.h> /* Use if you're brain's slow */
#include "ip_lib.h"
#include "bmp.h"

int main(){
	Bitmap* b = NULL;
	Bitmap* c = NULL;
	Bitmap* d = NULL;
	ip_mat* input_img = NULL;
	ip_mat* input_img2 = NULL;

	/* "Lab Rat" ip_mat */
	ip_mat* mod_ip_mat = NULL;

    /* Load a bmp image */
    b = bm_load("mongolfiere.bmp");
	input_img = bitmap_to_ip_mat(b);
	ip_mat_show(input_img);

    /* Test a function, show on screen the output, free up the memory and set the pointer to NULL*/
	mod_ip_mat = ip_mat_mul_scalar(input_img, 10.0);
	ip_mat_show(mod_ip_mat);
	d = ip_mat_to_bitmap(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
	mod_ip_mat = NULL;

	mod_ip_mat = ip_mat_add_scalar(input_img, 20.0);
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
	bm_save(d, "penismusic.bmp");
	bm_free(d);
	ip_mat_free(input_img);
	ip_mat_free(input_img2);

	return 0;
}

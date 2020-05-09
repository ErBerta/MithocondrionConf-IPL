#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

	float alpha = 0.0;
	char* filename = calloc(100, sizeof(*filename));

  /* Load a bmp image */
  b = bm_load("mongolfiere.bmp");
<<<<<<< HEAD
	c = bm_load("flower.bmp");
=======
	c = bm_load("penismusic.bmp");
>>>>>>> fa9926d2f04f7454fb39270ddfca6dec0958836f
	input_img = bitmap_to_ip_mat(b);
	input_img2 = bitmap_to_ip_mat(c);
	//ip_mat_show(input_img);

    /* Test a function, show on screen the output, free up the memory and set the pointer to NULL*/
<<<<<<< HEAD
	/*mod_ip_mat = ip_mat_mul_scalar(input_img, 10.0);
	ip_mat_show(mod_ip_mat);
	d = ip_mat_to_bitmap(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
	mod_ip_mat = NULL;
=======
	mod_ip_mat = ip_mat_mul_scalar(input_img, 10.0);
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}
>>>>>>> fa9926d2f04f7454fb39270ddfca6dec0958836f

	mod_ip_mat = ip_mat_add_scalar(input_img, 20.0);
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}


	mod_ip_mat = ip_mat_sum(input_img,input_img2);
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}

	mod_ip_mat = ip_mat_sub(input_img,input_img2);
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}

	mod_ip_mat = ip_mat_mean(input_img,input_img2);
<<<<<<< HEAD
	ip_mat_show(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
  mod_ip_mat = NULL;
*/
=======
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}

>>>>>>> fa9926d2f04f7454fb39270ddfca6dec0958836f
	/*mod_ip_mat = ip_mat_subset(input_img,0,92,0,93);
	ip_mat_show(mod_ip_mat);
	d = ip_mat_to_bitmap(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
  mod_ip_mat = NULL;*/

	/*mod_ip_mat = ip_mat_concat(input_img, input_img2,1);
	ip_mat_show(mod_ip_mat);
	d = ip_mat_to_bitmap(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
  mod_ip_mat = NULL;*/

	/*mod_ip_mat = ip_mat_create(5000, 5000, 3,0);
	ip_mat_init_random(mod_ip_mat,3,2.5);
	ip_mat_show(mod_ip_mat);
	d = ip_mat_to_bitmap(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
  mod_ip_mat = NULL;*/

/*	mod_ip_mat = ip_mat_to_gray_scale(input_img2);
	ip_mat_show(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
  mod_ip_mat = NULL;

<<<<<<< HEAD
	mod_ip_mat = ip_mat_brighten(input_img2,50);
=======
	mod_ip_mat = ip_mat_brighten(input_img, 60.0);
>>>>>>> fa9926d2f04f7454fb39270ddfca6dec0958836f
	ip_mat_show(mod_ip_mat);
	ip_mat_free(mod_ip_mat);
  mod_ip_mat = NULL;

  printf("Blending test\n");
  while(alpha <= 1.0) {
	  mod_ip_mat = ip_mat_blend(input_img, input_img2, alpha);

	  d = ip_mat_to_bitmap(mod_ip_mat);
	  sprintf(filename, "blending_%f.bmp", alpha);
	  bm_save(d, filename);
	  bm_free(d);
	  d = NULL;
	  bzero(filename, 100);

	  if(mod_ip_mat) {
		  ip_mat_free(mod_ip_mat);
		  mod_ip_mat = NULL;
		  printf("mod_ip_mat is OK\n");
	  }
	  else{
		  printf("mod_ip_mat is NULL\n");
	  }

	  alpha += 0.25;
  }
  printf("---- DONE\n");
	free(filename);
	filename = NULL;
	/* Don't free mod_ip_mat in this section here, but rather do it after each call to a function*/
	bm_free(b);
	bm_free(c);
	ip_mat_free(input_img);
	ip_mat_free(input_img2);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /* Measure exec time */
#include "ip_lib.h"
#include "bmp.h"

#define FDIM 100

/* Calculates elapsed time from when a function is executed to its end*/
double et(double start, double end){
	return ((double)(end - start))/ CLOCKS_PER_SEC;
}

int main(){
	clock_t start, end;

	Bitmap* b = NULL;
	Bitmap* c = NULL;
	Bitmap* d = NULL;
	ip_mat* input_img = NULL;
	ip_mat* input_img2 = NULL;

	/* "Lab Rat" ip_mat */
	ip_mat* mod_ip_mat = NULL;

	float alpha = 0.0, foo = 0.0;
	char* filename = calloc(FDIM, sizeof(*filename));
	char imgdir[] = "images";
	int i = 0, j = 0, z = 0, f = 0;

	/* Load a bmp image */
	sprintf(filename, "%s/mongolfiere.bmp", imgdir);
	b = bm_load(filename);
	input_img = bitmap_to_ip_mat(b);
	bzero(filename, FDIM);

	sprintf(filename, "%s/penismusic.bmp", imgdir);
	c = bm_load(filename);
	input_img2 = bitmap_to_ip_mat(c);
	bzero(filename, FDIM);

	/* Test a function, show on screen the output, free up the memory and set the pointer to NULL*/

	printf("_mul_scalar test\n");
	start = clock();
	mod_ip_mat = ip_mat_mul_scalar(input_img, 10.0);
	end = clock();
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}
	end = clock();
	printf("_mul_scalar took %f seconds\n", et(start, end));
	printf("---- DONE\n");

	printf("_add_scalar test\n");
	start = clock();
	mod_ip_mat = ip_mat_add_scalar(input_img, 20.0);
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}
	end = clock();
	printf("_add_scalar took %f seconds\n", et(start, end));
	printf("---- DONE\n");

	printf("_sum test\n");
	start = clock();
	mod_ip_mat = ip_mat_sum(input_img,input_img2);
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}
	end = clock();
	printf("_sum took %f seconds\n", et(start, end));
	printf("---- DONE\n");

	printf("_sub test\n");
	start = clock();
	mod_ip_mat = ip_mat_sub(input_img,input_img2);
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}
	end = clock();
	printf("_sub took %f seconds\n", et(start, end));
	printf("---- DONE\n");

	printf("_mean test\n");
	start = clock();
	mod_ip_mat = ip_mat_mean(input_img,input_img2);
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}
	end = clock();
	printf("_mean took %f seconds\n", et(start, end));
	printf("---- DONE\n");

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

	printf("Gray Scale test\n");
	start = clock();
	mod_ip_mat = ip_mat_to_gray_scale(input_img2);
	if(mod_ip_mat) {
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("mod_ip_mat is OK\n");
	}
	else{
		printf("mod_ip_mat is NULL\n");
	}
	printf("---- DONE\n");
	end = clock();
	printf("Gray scale took %f seconds\n", et(start, end));

	printf("Brighten test\n");
	i = 0;
	start = clock();
  while(foo <= 255.0){
	  mod_ip_mat = ip_mat_brighten(input_img, foo);
	  d = ip_mat_to_bitmap(mod_ip_mat);
	  sprintf(filename, "%s/brighten_%f.bmp", imgdir, foo);
	  bm_save(d, filename);
	  bzero(filename, FDIM);
	  bm_free(d);
	  d = NULL;
	  if(mod_ip_mat) {
		  ip_mat_free(mod_ip_mat);
		  mod_ip_mat = NULL;
		  printf("mod_ip_mat is OK\n");
	  }
	  else{
		  printf("mod_ip_mat is NULL\n");
	  }
	  foo += 25.0;
	  i++;
  }
  end = clock();
	printf("Total time to BRIGHTEN %d images: %f seconds\n", i, et(start, end));
	printf("---- DONE\n");

	printf("Blending test\n");
	start = clock();
	while(alpha <= 1.0) {
		mod_ip_mat = ip_mat_blend(input_img, input_img2, alpha);

		d = ip_mat_to_bitmap(mod_ip_mat);
		sprintf(filename, "%s/blending_%f.bmp", imgdir, alpha);
		bm_save(d, filename);
		bm_free(d);
		d = NULL;
		bzero(filename, FDIM);

		if(mod_ip_mat) {
			ip_mat_free(mod_ip_mat);
			mod_ip_mat = NULL;
			printf("mod_ip_mat is OK\n");
		}
		else{
			printf("mod_ip_mat is NULL\n");
		}

		alpha += 0.25;
		i++;
	}
	end = clock();
	printf("Total time to BLEND %d images: %f seconds\n", i, et(start, end));
	printf("---- DONE\n");

	alpha = (float)0.0;
	i = 0;
	start = clock();
	while(alpha <= 255.0){
		mod_ip_mat = ip_mat_corrupt(input_img2, alpha);
		d = ip_mat_to_bitmap(mod_ip_mat);
		sprintf(filename, "%s/corruption_%d.bmp", imgdir, (int)alpha);
		bm_save(d, filename);
		bzero(filename, FDIM);
		bm_free(d);
		d = NULL;
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;

		alpha += 15.0;
		i++;
	}
	end = clock();
	printf("Total time to CORRUPT %d images: %f seconds\n", i, et(start, end));

	printf("Concat test\n");
	for(i = 0; i < 3; i++){
		start = clock();
		mod_ip_mat = ip_mat_concat(input_img, input_img2, i);
		end = clock();
		d = ip_mat_to_bitmap(mod_ip_mat);
		sprintf(filename, "%s/concat_%d.bmp", imgdir, i);
		bm_save(d, filename);
		bzero(filename, FDIM);
		bm_free(d);
		d = NULL;
		ip_mat_free(mod_ip_mat);
		mod_ip_mat = NULL;
		printf("Time to Concat on %d-th dimension: %f seconds\n", i, et(start, end));
	}
	printf("---- DONE\n");

	printf("_padding test\n");
	start = clock();
	mod_ip_mat = ip_mat_padding(input_img, 25, 50);
	end = clock();
	d = ip_mat_to_bitmap(mod_ip_mat);
	sprintf(filename, "%s/padding.bmp", imgdir);
	bm_save(d, filename);
	bzero(filename, FDIM);
	bm_free(d);
	d = NULL;
	ip_mat_free(mod_ip_mat);
	mod_ip_mat = NULL;
	printf("_padding took: %f seconds\n", et(start, end));

	free(filename);
	filename = NULL;

	/* Don't free mod_ip_mat in this section here, but rather do it after each call to a function*/
	bm_free(b);
	bm_free(c);
	ip_mat_free(input_img);
	ip_mat_free(input_img2);

	return 0;
}

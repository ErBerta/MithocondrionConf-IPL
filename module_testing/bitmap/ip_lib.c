/*
 Created by Sebastiano Vascon on 23/03/20.
*/

#include <stdio.h>
#include "ip_lib.h"
#include "bmp.h"

void ip_mat_show(ip_mat * t){
    unsigned int i,l,j;
    printf("Matrix of size %d x %d x %d (hxwxk)\n",t->h,t->w,t->k);
    for (l = 0; l < t->k; l++) {
        printf("Slice %d\n", l);
        for(i=0;i<t->h;i++) {
            for (j = 0; j < t->w; j++) {
                printf("%f ", get_val(t,i,j,l));
            }
            printf("\n");
        }
        printf("\n");
    }
}

void ip_mat_show_stats(ip_mat * t){
    unsigned int k;

    compute_stats(t);

    for(k=0;k<t->k;k++){
        printf("Channel %d:\n", k);
        printf("\t Min: %f\n", t->stat[k].min);
        printf("\t Max: %f\n", t->stat[k].max);
        printf("\t Mean: %f\n", t->stat[k].mean);
    }
}

ip_mat * bitmap_to_ip_mat(Bitmap * img){
    unsigned int i=0,j=0;

    unsigned char R,G,B;

    unsigned int h = img->h;
    unsigned int w = img->w;

    ip_mat * out = ip_mat_create(h, w,3,0);

    for (i = 0; i < h; i++)              /* rows */
    {
        for (j = 0; j < w; j++)          /* columns */
        {
            bm_get_pixel(img, j,i,&R, &G, &B);
            set_val(out,i,j,0,(float) R);
            set_val(out,i,j,1,(float) G);
            set_val(out,i,j,2,(float) B);
        }
    }

    return out;
}

Bitmap * ip_mat_to_bitmap(ip_mat * t){

    Bitmap *b = bm_create(t->w,t->h);

    unsigned int i, j;
    for (i = 0; i < t->h; i++)              /* rows */
    {
        for (j = 0; j < t->w; j++)          /* columns */
        {
            bm_set_pixel(b, j,i, (unsigned char) get_val(t,i,j,0),
                    (unsigned char) get_val(t,i,j,1),
                    (unsigned char) get_val(t,i,j,2));
        }
    }
    return b;
}

float get_val(ip_mat * a, unsigned int i,unsigned int j,unsigned int k){
    if(i<a->h && j<a->w &&k<a->k){  /* j>=0 and k>=0 and i>=0 is non sense*/
        return a->data[i][j][k];
    }else{
        printf("Errore get_val!!!");
        exit(1);
    }
}

void set_val(ip_mat * a, unsigned int i,unsigned int j,unsigned int k, float v){
    if(i<a->h && j<a->w &&k<a->k){
        a->data[i][j][k]=v;
    }else{
        printf("Errore set_val!!!");
        exit(1);
    }
}

float get_normal_random(){
    float y1 = ( (float)(rand()) + 1. )/( (float)(RAND_MAX) + 1. );
    float y2 = ( (float)(rand()) + 1. )/( (float)(RAND_MAX) + 1. );
    return cos(2*PI*y2)*sqrt(-2.*log(y1));

}

/**** PARTE 1: TIPO DI DATI ip_mat E MEMORIA ****/
/* AUTHOR: Dussin */
ip_mat * ip_mat_create(unsigned int h, unsigned int w,unsigned  int k, float v){
    unsigned int i = 0, j = 0, z = 0;
    ip_mat* mat = (ip_mat*)calloc(1, sizeof(ip_mat));
    float*** data = (float***)calloc(h, sizeof(*data));
    stats* st = (stats*)calloc(k, sizeof(*st));
    mat->h = h;
    mat->w = w;
    mat->k = k;
    for(i = 0; i < h; i++){
        data[i] = (float**)calloc(w, sizeof(**data));
        for(j = 0; j < w; j++){
            data[i][j] = (float*)calloc(k, sizeof(***data));
            for(z = 0; z < k; z++){
                data[i][j][z] = v;
            }
        }
    }
    mat->data = data;
    mat->stat = st;

    return mat;
}

/* AUTHOR: Dussin */
void ip_mat_free(ip_mat *a){
	unsigned int i = 0, j = 0;
	for(i = 0; i < a->h; i++){
	    /* Free up each channel on the j-th row */
		for(j = 0; j < a->w; j++){
			free(a->data[i][j]);
			a->data[i][j] = NULL;
		}
		/* Free up the i-th row*/
		free(a->data[i]);
		a->data[i] = NULL;
	}
	/* Free up the whole data matrix, the stats and ultimately the whole struct*/
	free(a->data);
	a->data = NULL;
	free(a->stat);
	a->stat = NULL;
	free(a);
	a = NULL;
}

void compute_stats(ip_mat * t){
    printf("Compute stats is garbage\n");
}

/* AUTHOR: Dussin */
ip_mat * ip_mat_copy(ip_mat * in){
	ip_mat* m = NULL;
	unsigned int i, j, z;
	/* We have to literally copy the input matrix. We do NOT want to
	    affect the data in the input matrix, which would happen if we
	    merely copied the pointer to the input matrix.
	*/
	m = ip_mat_create(in->h, in->w, in->k, 0);
	for(i = 0; i < m->h; i++){
		for(j = 0; j < m->w; j++){
			for(z = 0; z < m->k; z++){
				m->data[i][j][z] = in->data[i][j][z];
			}
		}
	}
	m->stat->min = in->stat->min;
	m->stat->max = in->stat->max;
	m->stat->mean = in->stat->mean;
	return m;
}


/**** PARTE 1: OPERAZIONI MATEMATICHE FRA IP_MAT ****/
/* Esegue la somma di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output. */
 /* AUTHOR: Berta */
ip_mat * ip_mat_sum(ip_mat * a, ip_mat * b){
  ip_mat* tmp = NULL;
  if(a->w == b->w && a->h == b->h && a->k == b->k)
  {
    unsigned int i = 0, j = 0, z = 0;
  	tmp = ip_mat_copy(a);
  	for(i = 0; i < tmp->h; i++){
     for(j = 0; j < tmp->w; j++){
  		for(z = 0; z < tmp->k; z++){
  			tmp->data[i][j][z] += b->data[i][j][z];
  		}
  	 }
  	}
  }
	return tmp;
}
/* Esegue la sottrazione di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output.
 * */
 /* AUTHOR: Berta */
ip_mat * ip_mat_sub(ip_mat * a, ip_mat * b){
  ip_mat* tmp = NULL;
  if(a->w == b->w && a->h == b->h && a->k == b->k)
  {
    unsigned int i = 0, j = 0, z = 0;
  	tmp = ip_mat_copy(a);
  	for(i = 0; i < tmp->h; i++){
     for(j = 0; j < tmp->w; j++){
  		for(z = 0; z < tmp->k; z++){
  			tmp->data[i][j][z] -= b->data[i][j][z];
  		}
  	 }
  	}
  }
	return tmp;
}

/* Moltiplica un ip_mat per uno scalare c. Si moltiplica c per tutti gli elementi di "a"
 * e si salva il risultato in un nuovo tensore in output. */
/* AUTHOR: Dussin */
ip_mat * ip_mat_mul_scalar(ip_mat *a, float c){
	ip_mat* tmp = NULL;
	unsigned int i = 0, j = 0, z = 0;
	tmp = ip_mat_copy(a);
	for(i = 0; i < tmp->h; i++){
		for(j = 0; j < tmp->w; j++){
			for(z = 0; z < tmp->k; z++){
				tmp->data[i][j][z] *= c;
			}
		}
	}
	return tmp;
}

/* Aggiunge ad un ip_mat uno scalare c e lo restituisce in un nuovo tensore in output. */
/* AUTHOR: Dussin */
ip_mat *  ip_mat_add_scalar(ip_mat *a, float c){
	ip_mat* tmp = NULL;
	unsigned int i = 0, j = 0, z = 0;
	tmp = ip_mat_copy(a);
	for(i = 0; i < tmp->h; i++){
		for(j = 0; j < tmp->w; j++){
			for(z = 0; z < tmp->k; z++){
				tmp->data[i][j][z] += c;
			}
		}
	}
	return tmp;
}

/* Calcola la media di due ip_mat a e b e la restituisce in output.*/
/* AUTHOR: Berta */ /*TODO: controllare cast float*/
ip_mat * ip_mat_mean(ip_mat * a, ip_mat * b){
  ip_mat* tmp = NULL;
  unsigned int i = 0, j = 0, z = 0;
  tmp = ip_mat_copy(a);
  for(i = 0; i < tmp->h; i++){
   for(j = 0; j < tmp->w; j++){
  	for(z = 0; z < tmp->k; z++){
  		tmp->data[i][j][z] = (a->data[i][j][z] * b->data[i][j][z])/2.0;
  	}
   }
  }
  return tmp;
}

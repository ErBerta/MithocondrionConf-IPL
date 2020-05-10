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
/* Inizializza una ip_mat con dimensioni w h e k.
 * Ogni elemento è generato da una gaussiana con media mean e varianza var */
/* AUTHOR: Berta */
void ip_mat_init_random(ip_mat * t, float mean, float var)
{
	unsigned int i, j, z;
	const double pi = 3.141592653589793;
	for(i = 0; i < t->h; i++){
		for(j = 0; j < t->w; j++){
			for(z = 0; z < t->k; z++){
				t->data[i][j][z] = mean + var * (float) (sqrt ( - 2.0 * log ( get_normal_random() ) ) * cos ( 2.0 * pi * get_normal_random() ));
			}
		}
	}
}

/* Restituisce una sotto-matrice, ovvero la porzione individuata da:
 * t->data[row_start...row_end][col_start...col_end][0...k]
 * La terza dimensione la riportiamo per intero, stiamo in sostanza prendendo un sottoinsieme
 * delle righe e delle colonne.
 * */
/* AUTHOR: Berta */
ip_mat * ip_mat_subset(ip_mat * t, unsigned int row_start, unsigned int row_end, unsigned int col_start, unsigned int col_end)
{
	ip_mat* out = NULL;
	unsigned int i, j, z;

	out = ip_mat_create((row_end - row_start + 1), (col_end - col_start + 1), t->k, 0);
	for(i = row_start; i <= row_end; i++){
		for(j = col_start; j <= col_end; j++){
			for(z = 0; z < out->k; z++){
				out->data[i-row_start][j-col_start][z] = t->data[i][j][z];
			}
		}
	}
	return out;
}

/* Concatena due ip_mat su una certa dimensione.
 * Ad esempio:
 * ip_mat_concat(ip_mat * a, ip_mat * b, 0);
 *      produrrà un nuovo ip_mat di dimensioni:
 *      out.h = a.h + b.h
 *      out.w = a.w = b.w
 *      out.k = a.k = b.k
 *
 * ip_mat_concat(ip_mat * a, ip_mat * b, 1);
 *      produrrà un nuovo ip_mat di dimensioni:
 *      out.h = a.h = b.h
 *      out.w = a.w + b.w
 *      out.k = a.k = b.k
 *
 * ip_mat_concat(ip_mat * a, ip_mat * b, 2);
 *      produrrà un nuovo ip_mat di dimensioni:
 *      out.h = a.h = b.h
 *      out.w = a.w = b.w
 *      out.k = a.k + b.k
 * */
/*TODO: Serve controllare che le dimensioni siano uguali?*/
/*Meglio soluzione 1 o 2?*/
/* AUTHOR: Berta */
ip_mat * ip_mat_concat(ip_mat * a, ip_mat * b, int dimensione)
{
	ip_mat* out = NULL;
	unsigned int i, j, z;
	unsigned int h = a->h, w = a->w, k = a->k;
	unsigned int remh=0, remw=0, remk=0;

	switch(dimensione)
	{
		case 0:
			h = a->h + b->h;
			remh = a->h;
			break;
		case 1:
			w = a->w + b->w;
			remw = a->w;
			break;
		case 2:
			k = a->k + b->k;
			remk = a->k;
			break;
		default:
			return NULL;
			break;
	}
	out = ip_mat_create(h, w, k,0);
	for(i = 0; i < h; i++){
		for(j = 0; j < w; j++){
			for(z = 0; z < k; z++){
				if(i<a->h && j<a->w && z<a->k)
					out->data[i][j][z] = a->data[i][j][z];
				else
					out->data[i][j][z] = b->data[i-remh][j-remw][z-remk];
			}
		}
	}
	return out;
}

void set_val(ip_mat * a, unsigned int i,unsigned int j,unsigned int k, float v){
	if(i<a->h && j<a->w &&k<a->k){
		a->data[i][j][k]=v;
	}else{
		printf("Errore set_val!!!");
		exit(1);
	}
}

/* https://en.wikipedia.org/wiki/Normal_distribution#Generating_values_from_normal_distribution */
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
	unsigned int i, j, z;
	float sum[t->k];
	for(z = 0; z < t->k; z++) {
		sum[z] = 0;
	}

	for(i = 0; i < t->h; i++){
		for(j = 0; j < t->w; j++){
			for(z = 0; z < t->k; z++){
				t->stat[z].min = t->stat[z].min > t->data[i][j][z] ? t->data[i][j][z] : t->stat[z].min;
				t->stat[z].max = t->stat[z].max < t->data[i][j][z] ? t->data[i][j][z] : t->stat[z].max;
				sum[z] += t->data[i][j][z];
			}
		}
	}

	for(z = 0; z < t->k; z++) {
		t->stat[z].mean = sum[z] / (float)(t->h * t->w);
	}

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
	ip_mat* out = NULL;
	if(a->w == b->w && a->h == b->h && a->k == b->k)
	{
		unsigned int i = 0, j = 0, z = 0;
		out = ip_mat_create(a->h, a->w, a->k, 0);
		for(i = 0; i < out->h; i++){
			for(j = 0; j < out->w; j++){
				for(z = 0; z < out->k; z++){
					out->data[i][j][z] = a->data[i][j][z] + b->data[i][j][z];
				}
			}
		}
	}
	return out;
}
/* Esegue la sottrazione di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output.
 * */
/* AUTHOR: Berta */
ip_mat * ip_mat_sub(ip_mat * a, ip_mat * b){
	ip_mat* out = NULL;
	if(a->w == b->w && a->h == b->h && a->k == b->k)
	{
		unsigned int i = 0, j = 0, z = 0;
		out = ip_mat_create(a->h, a->w, a->k, 0);
		for(i = 0; i < out->h; i++){
			for(j = 0; j < out->w; j++){
				for(z = 0; z < out->k; z++){
					out->data[i][j][z] = a->data[i][j][z] - b->data[i][j][z];
				}
			}
		}
	}
	return out;
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
	ip_mat* out = NULL;
	unsigned int i = 0, j = 0, z = 0;
	out = ip_mat_create(a->h, a->w, a->k, 0);
	for(i = 0; i < out->h; i++){
		for(j = 0; j < out->w; j++){
			for(z = 0; z < out->k; z++){
				out->data[i][j][z] = (a->data[i][j][z] * b->data[i][j][z])/2.0;
			}
		}
	}
	return out;
}

/**** PARTE 2: SEMPLICI OPERAZIONI SU IMMAGINI ****/
/* Converte un'immagine RGB ad una immagine a scala di grigio.
 * Quest'operazione viene fatta calcolando la media per ogni pixel sui 3 canali
 * e creando una nuova immagine avente per valore di un pixel su ogni canale la media appena calcolata.
 * Avremo quindi che tutti i canali saranno uguali.
 * */
/* AUTHOR: Berta */
ip_mat * ip_mat_to_gray_scale(ip_mat * in){
	ip_mat* out = NULL;
	unsigned int i, j, z;
	float somma, media;
	out = ip_mat_create(in->h, in->w, in->k, 0);
	for(i = 0; i < out->h; i++){
		for(j = 0; j < out->w; j++){
			somma = 0;
			for(z = 0; z < out->k; z++){
				somma += in->data[i][j][z];
			}
			media = somma/out->k;
			for(z = 0; z < out->k; z++){
				out->data[i][j][z] = media;
			}
		}
	}
	return out;
}

/* Effettua la fusione (combinazione convessa) di due immagini */
/* AUTHOR: Dussin */
ip_mat * ip_mat_blend(ip_mat * a, ip_mat * b, float alpha){
	ip_mat* out = NULL;
	unsigned int i, j, z, _h, _w, _k;
	/* Pick the smaller dimensions */
	_h = a->h < b->h ? a->h : b->h;
	_w = a->w < b->w ? a->w : b->w;
	_k = a->k < b->k ? a->k : b->k;

	out = ip_mat_create(_h, _w, _k, 0);
	for(i = 0; i < _h; i++){
		for(j = 0; j < _w; j++){
			for(z = 0; z < _k; z++){
				out->data[i][j][z] = alpha * a->data[i][j][z] + (1 - alpha) * b->data[i][j][z];
			}
		}
	}
	return out;
}

/* Operazione di brightening: aumenta la luminosità dell'immagine
 * aggiunge ad ogni pixel un certo valore*/
/* AUTHOR: Berta */
ip_mat * ip_mat_brighten(ip_mat * in, float bright)
{
	ip_mat* out = NULL;
	unsigned int i, j, z;
	out = ip_mat_create(in->h, in->w, in->k, 0);
	for(i = 0; i < out->h; i++){
		for(j = 0; j < out->w; j++){
			for(z = 0; z < out->k; z++){
				out->data[i][j][z] = in->data[i][j][z] + bright;
				if(out->data[i][j][z] > 255.0){
				 out->data[i][j][z] = 255.0;
				}
				else if(out->data[i][j][z] < 0.0){
				 out->data[i][j][z] = 0.0;
				}
			}
		}
	}
	return out;
}

/* Operazione di corruzione con rumore gaussiano:
 * Aggiunge del rumore gaussiano all'immagine, il rumore viene enfatizzato
 * per mezzo della variabile amount.
 * out = a + gauss_noise*amount
 * */
/* AUTHOR: Dussin */
ip_mat * ip_mat_corrupt(ip_mat * a, float amount){
	ip_mat* out = NULL;
	unsigned int i, j, z;
	out = ip_mat_create(a->h, a->w, a->k, 0);
	for(i = 0; i < out->h; i++){
		for(j = 0; j < out->w; j++){
			for(z = 0; z < out->k; z++){
				out->data[i][j][z] = a->data[i][j][z] + get_normal_random()*amount;
				if(out->data[i][j][z] > 255.0){
					out->data[i][j][z] = 255.0;
				}
				else if(out->data[i][j][z] < 0.0){
					out->data[i][j][z] = 0.0;
				}
			}
		}
	}
	return out;
}

/**** PARTE 3: CONVOLUZIONE E FILTRI *****/
/* Aggiunge un padding all'immagine. Il padding verticale è pad_h mentre quello
 * orizzontale è pad_w.
 * L'output sarà un'immagine di dimensioni:
 *      out.h = a.h + 2*pad_h;
 *      out.w = a.w + 2*pad_w;
 *      out.k = a.k
 * con valori nulli sui bordi corrispondenti al padding e l'immagine "a" riportata
 * nel centro
 * */
/* AUTHOR: Dussin */
ip_mat * ip_mat_padding(ip_mat * a, int pad_h, int pad_w){
	ip_mat* out = NULL;
	unsigned int i, j, z;
	out = ip_mat_create(a->h + 2*pad_h, a->w + 2*pad_w, a->k, 0);
	for(i = pad_h; i < pad_h + a->h; i++){
		for(j = pad_w; j < pad_w + a->w; j++) {
			for(z = 0; z < out->k; z++) {
				out->data[i][j][z] = a->data[i - pad_h][j - pad_w][z];
			}
		}
	}
	return out;
}


/*
 * Id gruppo: 43
 * 
 * Componenti:
 *  Elia Bertapelle, 881359
 *  Alessandro Dussin, 881424
 *  Tomas Fornasa, 880034
 *  Leonardo Piccolo, 882351
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

	if(!t){
		fprintf(stderr, "Error: ip_mat pointer is NULL in ip_mat_show_stats\n");
		exit(1);
	}
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

	if(!img){
		fprintf(stderr, "Error: Bitmap pointer is NULL in bitmap_to_ip_mat\n");
		exit(1);
	}
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

    compute_stats(out);

    return out;
}

Bitmap * ip_mat_to_bitmap(ip_mat * t){

	if(!t){
		fprintf(stderr, "Error: ip_mat pointer is NULL in ip_mat_to_bitmap\n");
		exit(1);
	}
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
	if(!a){
		fprintf(stderr, "Error: ip_mat pointer is NULL in get_val\n");
		exit(1);
	}
    if(i<a->h && j<a->w &&k<a->k){
        return a->data[i][j][k];
    }else{
        printf("Errore get_val!!!");
        exit(1);
    }
}

void set_val(ip_mat * a, unsigned int i,unsigned int j,unsigned int k, float v){
	if(!a){
		fprintf(stderr, "Error: ip_mat pointer is NULL in set_val\n");
		exit(1);
	}
    if(i<a->h && j<a->w &&k<a->k){
        a->data[i][j][k]=v;
    }else{
        printf("Errore set_val!!!");
        exit(1);
    }
}

float get_normal_random(float media, float std){

    float y1 = ( (float)(rand()) + 1. )/( (float)(RAND_MAX) + 1. );
    float y2 = ( (float)(rand()) + 1. )/( (float)(RAND_MAX) + 1. );
    float num = cos(2* M_PI *y2) * sqrt(-2. * log(y1));

    return media + num*std;
}

/* Inizializza una ip_mat con dimensioni w h e k.
 * Ogni elemento è generato da una gaussiana con media mean e varianza var */

void ip_mat_init_random(ip_mat * t, float mean, float var)
{
	unsigned int i, j, z;
	if(!t){
		fprintf(stderr, "Error: ip_mat pointer is NULL in ip_mat_init_random\n");
		exit(1);
	}
	for(i = 0; i < t->h; i++){
		for(j = 0; j < t->w; j++){
			for(z = 0; z < t->k; z++){
				t->data[i][j][z] = mean + var * (float) (sqrt ( - 2.0 * log ( get_normal_random(mean, var) ) ) * cos ( 2.0 * M_PI * get_normal_random(mean, var) ));
			}
		}
	}
}

/* Restituisce una sotto-matrice, ovvero la porzione individuata da:
 * t->data[row_start...row_end][col_start...col_end][0...k]
 * La terza dimensione la riportiamo per intero, stiamo in sostanza prendendo un sottoinsieme
 * delle righe e delle colonne.
 * */

ip_mat * ip_mat_subset(ip_mat * t, unsigned int row_start, unsigned int row_end, unsigned int col_start, unsigned int col_end)
{
	ip_mat* out = NULL;
	unsigned int i, j, z;
	if(!t){
		fprintf(stderr, "Error: ip_mat pointer is NULL in ip_mat_subset\n");
		exit(1);
	}
	/*controllo che le dimensioni inserite siano valide*/
	if(row_start >= 0 && row_end <= t->h && col_start >= 0 && col_end <= t->w)
	{
		out = ip_mat_create((row_end - row_start + 1), (col_end - col_start + 1), t->k, 0);
		for(i = row_start; i <= row_end; i++){
			for(j = col_start; j <= col_end; j++){
				for(z = 0; z < out->k; z++){
					out->data[i-row_start][j-col_start][z] = t->data[i][j][z];
				}
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

ip_mat * ip_mat_concat(ip_mat * a, ip_mat * b, int dimensione)
{
	ip_mat* out = NULL;
	unsigned int i, j, z;
	unsigned int h = a->h, w = a->w, k = a->k;
	unsigned int remh=0, remw=0, remk=0;
	/*int usato come bool per controllare se è possibile effettuare l'operazione*/
	int valid = 1;

	if(!a || !b){
		fprintf(stderr, "Error: ip_mat pointer is NULL in \n");
		exit(1);
	}
	switch(dimensione)
	{
		case 0:
			h = a->h + b->h;
			remh = a->h;
			if(a->w != b->w || a->k != b->k)
				valid = 0;
			break;
		case 1:
			w = a->w + b->w;
			remw = a->w;
			if(a->h != b->h || a->k != b->k)
				valid = 0;
			break;
		case 2:
			k = a->k + b->k;
			remk = a->k;
			if(a->w != b->w || a->h != b->h)
				valid = 0;
			break;
		default:
			return NULL;
			break;
	}
	/*se ottengo delle dimensioni coerenti procedo, altrimenti restituistco un puntatore a NULL*/
	if(valid)
	{
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
	}
	else{
			fprintf(stderr, "Error: Invalid dimension in concat\n");
			exit(1);
	}
	return out;
}

/**** PARTE 1: TIPO DI DATI ip_mat E MEMORIA ****/

ip_mat * ip_mat_create(unsigned int h, unsigned int w,unsigned  int k, float v){
	unsigned int i = 0, j = 0, z = 0;
	ip_mat* mat = (ip_mat*)calloc(1, sizeof(ip_mat));
	if(!mat){
		fprintf(stderr, "Error: Couldn't allocate enough memory to mat in ip_mat_create\n");
		exit(1);
	}
	float*** data = (float***)calloc(h, sizeof(*data));
	if(!data){
		fprintf(stderr, "Error: Couldn't allocate enough memory to data in ip_mat_create\n");
		exit(1);
	}
	stats* st = (stats*)calloc(k, sizeof(*st));
	if(!st){
		fprintf(stderr, "Error: Couldn't allocate enough memory to st in ip_mat_create\n");
		exit(1);
	}

	mat->h = h;
	mat->w = w;
	mat->k = k;
	for(i = 0; i < h; i++){
		data[i] = (float**)calloc(w, sizeof(**data));
		if(!data[i]){
			fprintf(stderr, "Error: Couldn't allocate enough memory to data[i] in ip_mat_create\n");
			exit(1);
		}

		for(j = 0; j < w; j++){
			data[i][j] = (float*)calloc(k, sizeof(***data));
			if(!data[i][j]){
				fprintf(stderr, "Error: Couldn't allocate enough memory to data[i][j] in ip_mat_create\n");
				exit(1);
			}
			for(z = 0; z < k; z++){
				data[i][j][z] = v;
			}
		}
	}
	mat->data = data;
	mat->stat = st;
	mat->stat->min = v;
	mat->stat->max = v;
	mat->stat->mean = v;

	return mat;
}

/* Libera la memoria (data, stat e la struttura)
 *
 * se la variabile "a" è NULL non fa nulla.
 *
 * */

void ip_mat_free(ip_mat *a){
	unsigned int i = 0, j = 0;
	if(!a){
		fprintf(stderr, "Error: ip_mat pointer is NULL in ip_mat_free\n");
		exit(1);
	}

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
	if (t != NULL)
	{
		for (z = 0; z < t->k; z++)
		{
			t->stat[z].min = FLT_MAX;
			t->stat[z].max = 0;
			t->stat[z].mean = 0;
		}
		for (i = 0; i < t->h; i++)
		{
			for (j = 0; j < t->w; j++)
			{
				for (z = 0; z < t->k; z++)
				{
					t->stat[z].min = t->stat[z].min > t->data[i][j][z] ? t->data[i][j][z] : t->stat[z].min;
					t->stat[z].max = t->stat[z].max < t->data[i][j][z] ? t->data[i][j][z] : t->stat[z].max;
					t->stat[z].mean += t->data[i][j][z];
				}
			}
		}

		for (z = 0; z < t->k; z++)
		{
			t->stat[z].mean /= (float)(t->h * t->w);
		}
	}
	else
	{
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in compute_stats!!!\n");
		exit(1);
	}
}


ip_mat * ip_mat_copy(ip_mat * in){
	ip_mat* m = NULL;
	unsigned int i, j, z;

	if (in != NULL)
	{
		/* We have to literally copy the input matrix. We do NOT want to
	    affect the data in the input matrix, which would happen if we
	    merely copied the pointer to the input matrix.
		*/

		m = ip_mat_create(in->h, in->w, in->k, 0);
		for (i = 0; i < m->h; i++)
		{
			for (j = 0; j < m->w; j++)
			{
				for (z = 0; z < m->k; z++)
				{
					m->data[i][j][z] = in->data[i][j][z];
				}
			}
		}
		m->stat->min = in->stat->min;
		m->stat->max = in->stat->max;
		m->stat->mean = in->stat->mean;
		return m;
	}
	else
	{
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_copy!!!\n");
		exit(1);
	}
}


/**** PARTE 1: OPERAZIONI MATEMATICHE FRA IP_MAT ****/
/* Esegue la somma di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output. */

ip_mat * ip_mat_sum(ip_mat * a, ip_mat * b){
	ip_mat* out = NULL;
	if(a != NULL && b != NULL)
	{
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
	}
	else
	{
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_sum!!!\n");
		exit(1);
	}
	return out;
}
/* Esegue la sottrazione di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output.
 * */

ip_mat * ip_mat_sub(ip_mat * a, ip_mat * b){
	ip_mat* out = NULL;
	if (a != NULL && b != NULL)
	{
		if (a->w == b->w && a->h == b->h && a->k == b->k)
		{
			unsigned int i = 0, j = 0, z = 0;
			out = ip_mat_create(a->h, a->w, a->k, 0);
			for (i = 0; i < out->h; i++)
			{
				for (j = 0; j < out->w; j++)
				{
					for (z = 0; z < out->k; z++)
					{
						out->data[i][j][z] = a->data[i][j][z] - b->data[i][j][z];
					}
				}
			}
		}
	}
	else
	{
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_sub!!!\n");
		exit(1);
	}

	return out;
}

/* Moltiplica un ip_mat per uno scalare c. Si moltiplica c per tutti gli elementi di "a"
 * e si salva il risultato in un nuovo tensore in output. */

ip_mat * ip_mat_mul_scalar(ip_mat *a, float c){
	ip_mat* tmp = NULL;
	unsigned int i = 0, j = 0, z = 0;
	if (a != NULL)
	{
		tmp = ip_mat_copy(a);
		for (i = 0; i < tmp->h; i++)
		{
			for (j = 0; j < tmp->w; j++)
			{
				for (z = 0; z < tmp->k; z++)
				{
					tmp->data[i][j][z] *= c;
				}
			}
		}
	}
	else
	{
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_mul_scalar!!!\n");
		exit(1);
	}
	return tmp;
}

/* Aggiunge ad un ip_mat uno scalare c e lo restituisce in un nuovo tensore in output. */

ip_mat *  ip_mat_add_scalar(ip_mat *a, float c){
	ip_mat* tmp = NULL;
	unsigned int i = 0, j = 0, z = 0;

	if (a != NULL)
	{
		tmp = ip_mat_copy(a);
		for (i = 0; i < tmp->h; i++)
		{
			for (j = 0; j < tmp->w; j++)
			{
				for (z = 0; z < tmp->k; z++)
				{
					tmp->data[i][j][z] += c;
				}
			}
		}
	}
	else
	{
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_add_scalar!!!\n");
		exit(1);
	}
	return tmp;
}

/* Calcola la media di due ip_mat a e b e la restituisce in output.*/

ip_mat * ip_mat_mean(ip_mat * a, ip_mat * b){
	ip_mat* out = NULL;
	unsigned int i = 0, j = 0, z = 0;

	if(a->w == b->w && a->h == b->h && a->k == b->k)
	{
		out = ip_mat_create(a->h, a->w, a->k, 0);
		for(i = 0; i < out->h; i++){
			for(j = 0; j < out->w; j++){
				for(z = 0; z < out->k; z++){
					out->data[i][j][z] = (a->data[i][j][z] * b->data[i][j][z])/2.0;
				}
			}
		}
	}
	else
	{
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_mean!!!\n");
		exit(1);
	}

	return out;
}

/**** PARTE 2: SEMPLICI OPERAZIONI SU IMMAGINI ****/
/* Converte un'immagine RGB ad una immagine a scala di grigio.
 * Quest'operazione viene fatta calcolando la media per ogni pixel sui 3 canali
 * e creando una nuova immagine avente per valore di un pixel su ogni canale la media appena calcolata.
 * Avremo quindi che tutti i canali saranno uguali.
 * */

ip_mat * ip_mat_to_gray_scale(ip_mat * in){

	ip_mat* out = NULL;
	unsigned int i, j, z;
	float somma, media;

    if (in == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_to_gray_scale");
        exit(1);
    }

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

/* Effettua la fusione (combinazione convessa) di due immagini.
 *
 * I parametri della funzione non subiscono modiche, il risultato viene salvato e restituito in output
 * all'interno di una nuova ip_mat.
 *
 * Le variabili "a" e "b" devono avere le stesse dimensioni
 */

ip_mat * ip_mat_blend(ip_mat * a, ip_mat * b, float alpha){
	ip_mat* out = NULL;
	unsigned int i, j, z;

    if (a == NULL || b == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_blend");
        exit(1);
    }

    if (alpha < 0) {
        fprintf(stderr, "ERROR: invalid value for alpha in ip_mat_blend");
        exit(1);
    }

    if(a->h != b->h || a->w != b->w || a->k != b->k)
    {
        fprintf(stderr, "ERROR: size mismatch in ip_mat_blend");
        exit(1);
    }

    out = ip_mat_create(a->h, a->w, a->k, 0);
    for(i = 0; i < a->h; i++){
        for(j = 0; j < a->w; j++){
            for(z = 0; z < a->k; z++){
                out->data[i][j][z] = alpha * a->data[i][j][z] + (1 - alpha) * b->data[i][j][z];
            }
        }
    }
	return out;
}

/* Operazione di brightening: aumenta la luminosità dell'immagine
 * aggiunge ad ogni pixel un certo valore*/

ip_mat * ip_mat_brighten(ip_mat * in, float bright)
{
	ip_mat* out = NULL;
	unsigned int i, j, z;

    if (in == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_brighten");
        exit(1);
    }

	out = ip_mat_create(in->h, in->w, in->k, 0);
	for(i = 0; i < out->h; i++){
		for(j = 0; j < out->w; j++){
			for(z = 0; z < out->k; z++){
				out->data[i][j][z] = in->data[i][j][z] + bright;
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

ip_mat * ip_mat_corrupt(ip_mat * a, float amount){
	ip_mat* out = NULL;
	unsigned int i, j, z;

    if (a == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_corrupt");
        exit(1);
    }

	out = ip_mat_create(a->h, a->w, a->k, 0);
	for(i = 0; i < out->h; i++){
		for(j = 0; j < out->w; j++){
			for(z = 0; z < out->k; z++){
				out->data[i][j][z] = a->data[i][j][z] + get_normal_random(0, amount);
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

ip_mat * ip_mat_padding(ip_mat * a, unsigned int pad_h, unsigned int pad_w){
	ip_mat* out = NULL;
	unsigned int i, j, z;

	if(a == NULL) {
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_padding\n");
		exit(1);
	}

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

/* Effettua la convoluzione di un ip_mat "a" con un ip_mat "f".
 * La funzione restituisce un ip_mat delle stesse dimensioni di "a".
 * */
ip_mat * ip_mat_convolve(ip_mat * a, ip_mat * f) {

    if (a == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_convolve");
        exit(1);
    }

    if (f == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat filter is NULL in ip_mat_convolve");
        exit(1);
    }

    ip_mat* out = NULL;
    unsigned pad_h = (f->h - 1)/2;
    unsigned pad_w = (f->w - 1)/2;

    ip_mat* in = ip_mat_padding(a, pad_h, pad_w);
    out = ip_mat_create(a->h, a->w, a->k, 0);

    unsigned int i, j, z, m, n;

    for (j = 0; j < out->h ; j++) {
        for (i = 0; i < out->w ; i++) {
            for (z = 0; z < out->k; z++) {
                float sum = 0;
                for(n = 0; n < f->h && n + j < in->h; n++) {
                    for(m = 0; m < f->w && m + i < in->w; m++) {
                        sum += (get_val(in, j + n, i + m, z) * get_val(f, n, m, z));
                    }
                }

                out->data[j][i][z] = sum;
            }
        }
    }

    ip_mat_free(in);
    in = NULL;

    return out;
}

/* Crea un filtro di sharpening */
ip_mat * create_sharpen_filter() {
    ip_mat* out = ip_mat_create(3, 3, 3, 0);

    unsigned int z;

    for (z = 0; z < out->k; z++) {
        out->data[0][1][z] = -1;
        out->data[1][0][z] = -1;
        out->data[1][1][z] = 5;
        out->data[1][2][z] = -1;
        out->data[2][1][z] = -1;
    }

    return out;
}

/* Crea un filtro per rilevare i bordi */
ip_mat * create_edge_filter() {
    ip_mat* out = ip_mat_create(3, 3, 3, -1);

    unsigned int z;
    for (z=0; z < out->k; z++)
        out->data[1][1][z] = 8;

    return out;
}

/* Crea un filtro per aggiungere profondità */
ip_mat * create_emboss_filter() {
    ip_mat* out = ip_mat_create(3, 3, 3, 0);

    unsigned int z;

    for (z = 0; z < out->k; z++) {
        out->data[0][0][z] = -2;
        out->data[0][1][z] = -1;
        out->data[1][0][z] = -1;
        out->data[1][1][z] = 1;
        out->data[1][2][z] = 1;
        out->data[2][1][z] = 1;
        out->data[2][2][z] = 2;
    }

    return out;
}

/* Crea un filtro medio per la rimozione del rumore */
ip_mat * create_average_filter(unsigned int w, unsigned int h, unsigned int k) {

    float c = 1.0/(w*h);

    return ip_mat_create(h, w, k, c);
}

/* Crea un filtro gaussiano per la rimozione del rumore */
ip_mat * create_gaussian_filter(unsigned int w, unsigned int h, unsigned int k, float sigma){
    ip_mat* out = ip_mat_create(h, w, k, 0);

    unsigned int x, y, z;
    unsigned int cx = w/2, cy = h/2;
    float constant = 1/(2* M_PI * pow(sigma, 2));

    for (x = 0; x < out->w; x++) {
        for (y = 0; y < out->h; y++) {
            for (z = 0; z < out->k; z++) {
                int mx = x - cx;
                int my = y - cy;
                out->data[y][x][z] = constant * exp( - (pow(mx, 2) + pow(my, 2)) / (2 * pow(sigma, 2)));
            }
        }
    }

    return out;
}

/* Nell'operazione di clamping i valori <low si convertono in low e i valori >high in high.*/
void clamp(ip_mat * t, float low, float high){

    unsigned int x, y, z;

	if(t == NULL) {
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in clamp\n");
		exit(1);
	}

    for(x = 0; x < t->w; x++) {
        for(y = 0; y < t->h; y++) {
            for(z = 0; z < t->k; z++) {
                float data = t->data[y][x][z];
                if (data > high) {
                    t->data[y][x][z] = high;
                }
                if (data < low) {
                    t->data[y][x][z] = low;
                }
            }
        }
    }
}

/* Effettua una riscalatura dei dati tale che i valori siano in [0,new_max].
 * Utilizzate il metodo compute_stat per ricavarvi il min, max per ogni canale.
 *
 * I valori sono scalati tramite la formula valore-min/(max - min)
 *
 * Si considera ogni indice della terza dimensione indipendente, quindi l'operazione
 * di scalatura va ripetuta per ogni "fetta" della matrice 3D.
 * Successivamente moltiplichiamo per new_max gli elementi della matrice in modo da ottenere un range
 * di valori in [0,new_max].
 * */
void rescale(ip_mat * t, float new_max){

    unsigned int x, y, z;

	if(t == NULL) {
		fprintf(stderr, "ERROR: pointer to ip_mat is NULL in rescale\n");
		exit(1);
	}

    compute_stats(t);

    for(x = 0; x < t->w; x++) {
        for(y = 0; y < t->h; y++) {
            for(z = 0; z < t->k; z++) {
				t->data[y][x][z] = ((t->data[y][x][z] - t->stat[z].min)/(t->stat[z].max - t->stat[z].min)) * new_max;
            }
        }
    }
}

/**** PARTE 4: NOSTRE IDEE: METODI E FUNZIONI IN PIU' *****/
/* Crea un immagine ridimensionata alle dimensioni volute
 * */
ip_mat * ip_mat_resize(ip_mat* a, unsigned int new_height, unsigned int new_width) {
    ip_mat* out = NULL;
    unsigned int i, j, z;

    if(a == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_resize\n");
        exit(1);
    }

    out = ip_mat_create(new_height, new_width, a->k, 0);

    float nh = (float)a->h / (float)new_height ;
    float nw = (float)a->w / (float)new_width;

    for(i = 0; i < out->h; i++){
        for(j = 0; j < out->w; j++){
            for(z = 0; z < out->k; z++){
                unsigned int o_i = i * nh;
                unsigned int o_j = j * nw;
                out->data[i][j][z] = a->data[o_i][o_j][z];
            }
        }
    }
    return out;
}

/*
 *  Crea un immagine sostituendo dall'immagine a il colore color (nell'intorno di precision) con i valori dell'immagine bg
 *  a e bg devono avere la stessa dimensione
 *  color e precision devono avere la stessa dimensione di a e bg
 */
ip_mat * background_chroma_key(ip_mat* a, ip_mat* bg, float* color, float* precision) {
    ip_mat* out = NULL;
    unsigned int i, j, z, n;

    if(a == NULL || bg == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat is NULL in background_chroma_key\n");
        exit(1);
    }
	
	if(a->w < bg->w || a->h < bg->h || a->k < bg->k){
		fprintf(stderr, "ERROR: pointer to ip_mat b is smaller than a in background_chroma_key\n");
        exit(1);
	}
    
	if(color == NULL || precision == NULL) {
        fprintf(stderr, "ERROR: pointer is NULL in background_chroma_key\n");
        exit(1);
    }

    out = ip_mat_copy(a);

    for(i = 0; i < out->h; i++){
        for(j = 0; j < out->w; j++){
            for(z = 0, n = 0; z < out->k; z++){
                if (out->data[i][j][z] > color[z] - precision[z] && out->data[i][j][z] < color[z] + precision[z]) {
                    n++;
                }
            }
            if (n != 0) {
                for(z = 0; z < out->k; z++){
                    out->data[i][j][z] = bg->data[i][j][z];
                }
            }

        }
    }
    return out;
}

/*
 *  Crea un immagine in scala di grigi, applicata a tutti i colori eccetto quello specificato da color (nell'intorno di precision)
 */
ip_mat * grey_scale_chroma_key(ip_mat* in, float* color, float* precision) {
    ip_mat* out = NULL;
    unsigned int i, j, z, n=0;

    if(in == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat is NULL in grey_scale_chroma_key\n");
        exit(1);
    }

    if(color == NULL || precision == NULL) {
        fprintf(stderr, "ERROR: pointer is NULL in grey_scale_chroma_key\n");
        exit(1);
    }

    float somma, media;
    out = ip_mat_copy(in);

    for(i = 0; i < out->h; i++){
        for(j = 0; j < out->w; j++){
            somma = 0;
            n = 0;
            for(z = 0; z < out->k; z++) {
                somma += out->data[i][j][z];
                if (out->data[i][j][z] > color[z] - precision[z] && out->data[i][j][z] < color[z] + precision[z]) {
                    n++;
                }
            }
            media = somma/out->k;
            for(z = 0; z < out->k; z++) {
                if (n != out->k) {
                    out->data[i][j][z] = media;
                }
            }
        }
    }


    return out;
}

/*
 * Aumenta o riduce il contrasto di un ip_mat
 */
ip_mat * ip_mat_contrast(ip_mat* in, float contrast)
{
    ip_mat* out = NULL;
    unsigned int i, j, z;

    if(in == NULL) {
        fprintf(stderr, "ERROR: pointer to ip_mat is NULL in ip_mat_contrast\n");
        exit(1);
    }

    if(contrast < 0) {
        fprintf(stderr, "ERROR: invalid value for contrast in ip_mat_contrast\n");
        exit(1);
    }

    out = ip_mat_copy(in);

    for(i = 0; i < out->h; i++){
        for(j = 0; j < out->w; j++){
            for(z = 0; z < out->k; z++) {
                out->data[i][j][z] = (((((out->data[i][j][z] / 255.0) - 0.5) * contrast) + 0.5) * 255.0);
            }
        }
    }
    return out;
}

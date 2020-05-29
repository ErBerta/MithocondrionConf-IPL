#include <stdio.h>
#include <string.h>
#include "ip_lib.h"
#include "bmp.h"

void show_help(){
    printf("*** Image Processing Toolbox ***\n");
    printf("\targ 1: input file name (img1) \n");
    printf("\targ 2: input file name (img2) \n");
    printf("\targ 3: operazione da effettuare (corrupt, gray, brighten, blend, sharp, edge, emboss, avg, gauss) \n");
    printf("\targ 4: output file name\n");
    printf("\targ 5: Se 1 concatena la/le immagini di input con quella di output\n");
    printf("\targ 6: Diversi significati in funzione dell'operazione (default 3):\n"
           "\t\t- [avg, gauss]: kernel size \n"
           "\t\t- [corrupt]: massimo livello di noise se si vuole corrompere l'immagine\n"
           "\t\t- [brighten]: valore bright per aumentare la luminosità \n"
           "\t\t\n");
    printf("\targ 7: Diversi significati in funzione dell'operazione (default 1.0):\n"
           "\t\t- [gauss] parametro sigma del kernel Gaussiano\n"
           "\t\t- [blend] parametro alpha per il blending di due immagini");
    printf("\n");
}

int main (int argc, char * argv[]) {

    char * fn_in_1;  /* file 1 */
    char * fn_in_2;  /* file 2 */
    char * operation; /* operazione da eseguire */
    char * fn_out; /* output file */

    int concat_images = 0; /* concatena o meno le immagini in output */

    int k_size = 3; /* kernel size */
    float sigma = 1.; /* sigma del kernel gaussiano */

    /* variabili di appoggio per le computazioni */
    Bitmap * b = NULL, *c = NULL, *b2 = NULL;
    ip_mat * input_img = NULL;
    ip_mat * filter = NULL;
    ip_mat * img= NULL, * img_b= NULL;
    ip_mat * temp = NULL;

    if(argc==1){
        show_help();
        return 0;
    }

    if(argc<5){
        printf("Non sono stati forniti i parametri obbligatori!\n");
        return 0;
    }

    fn_in_1 = argv[1];  /* file 1 */
    fn_in_2 = argv[2];  /* file 2 */
    operation = argv[3]; /* operazione da eseguire */
    fn_out = argv[4]; /* output file */

    if(argc>5) {
        concat_images = atoi(argv[5]);
    }

    if(argc>6){
        k_size = atoi(argv[6]);
    }

    if(argc>7){
        sigma = atof(argv[7]);
    }

    b = bm_load(fn_in_1);  /* leggi il file di input */

    input_img = bitmap_to_ip_mat(b); /* converti la bitmap in un ip_mat */

    bm_free(b); /* libera la memoria dalla bitmap, da qui in poi lavoriamo con ip_mat */

    if (strcmp(operation, "corrupt") == 0) {
        img = ip_mat_corrupt(input_img, k_size);  /* corrompi l'immagine con del rumore */
        clamp(img,0,255); /* effettua il clamping dei valori in 0-255 */
    }
    else if (strcmp(operation, "brighten") == 0) {
        img = ip_mat_brighten(input_img, k_size); /* aumenta la luminosità */
        clamp(img,0,255); /* effettua il clamping dei valori in 0-255 */
    }
    else if (strcmp(operation, "blend") == 0) {
        Bitmap * c = bm_load(fn_in_2);
        ip_mat * img_b = bitmap_to_ip_mat(c);

        img = ip_mat_blend(input_img, img_b, sigma); /* effettua il blending di due immagini */

        ip_mat_free(img_b);
        bm_free(c);
    }else if (strcmp(operation, "gray") == 0) {
        img = ip_mat_to_gray_scale(input_img);
    }
    else if (strcmp(operation, "sharp") == 0) {
        filter = create_sharpen_filter(); /* crea un filtro di sharpening */
        img = ip_mat_convolve(input_img, filter); /* applica la convoluzione */
        clamp(img,0,255);  /* effettua il clamping dei valori in 0-255 */
    }
    else if (strcmp(operation, "edge") == 0) {
        filter = create_edge_filter();
        img = ip_mat_convolve(input_img, filter);
        clamp(img,0,255);
    } else if (strcmp(operation, "emboss") == 0) {
        filter = create_emboss_filter();
        img = ip_mat_convolve(input_img, filter);
        clamp(img,0,255);
    } else if (strcmp(operation, "avg") == 0) {
        filter = create_average_filter(k_size, k_size, 3);
        img = ip_mat_convolve(input_img, filter);
    } else if (strcmp(operation, "gauss") == 0) {
        filter = create_gaussian_filter(k_size, k_size, 3, sigma);
        img = ip_mat_convolve(input_img, filter);
        clamp(img,0,255);
    } else {
        printf("The required operation doesn't exists\n");
        exit(1);
    }

    if(concat_images) {
        if(strcmp(operation, "blend") == 0){
            c = bm_load(fn_in_2);
            img_b = bitmap_to_ip_mat(c);
            temp = ip_mat_concat(input_img, img_b, 1);
            ip_mat_free(img_b);
            img_b = ip_mat_concat(temp, img, 1);
            ip_mat_free(temp);
            temp = img_b;
            bm_free(c);
        }else{
            temp = ip_mat_concat(input_img, img, 1); /* metti le due immagini vicine */
        }
        ip_mat_free(img);  /* libera la memoria da img */
        img = temp;
    }

    ip_mat_free(input_img); /* libera la memoria dalla ip_mat contenente l'immagine di input */

    b2 = ip_mat_to_bitmap(img); /* converti l'immagine di output in una bitmap */

    ip_mat_free(img); /* libera la memoria da img */
    ip_mat_free(filter); /* libera la memoria dal filtro */

    bm_save(b2, fn_out); /* salva la bitmap di output su file */
    bm_free(b2); /* libera la memoria dalla bitmap */

    return 0; /* ciao a tutti!*/
}

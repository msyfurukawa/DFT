#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "MT.h"
#include "CmpNum.h"

//execute DFT and FFT in init_k <= k <= k_max
#define init_k 3
#define k_max 12

//execute DFT and FFT ITER times in one k
#define ITER 30

#define OUTPUT_LIM 20

//discrete Fourier transform according to definition formula
void DFT(complex *data, complex *X, int K);

//fast Fourier transform
void FFT(complex *data, complex *X, int k);

//return order number(char array) of i
void ord_num(int i, char *result){
	sprintf(result, "%d", i);
	if(i % 10 == 1){
		strcat(result, "st");
	}else if(i % 10 == 2){
		strcat(result, "nd");
	}else if(i % 10 == 3){
		strcat(result, "rd");
	}else{
		strcat(result, "th");
	}
}

//get micro second
long get_usec(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long)tv.tv_sec*1000*1000 + (long)tv.tv_usec;
}

int main(){
	//K_max = 2^{k_max}
	int K_max = (int)pow(2, k_max);

	//objective data for Fourier transform
	complex *a = (complex*)malloc(sizeof(complex)*K_max);

	//store the result of DFT & FFT
	complex *X1 = (complex*)malloc(sizeof(complex)*K_max);
	complex *X2 = (complex*)malloc(sizeof(complex)*K_max);

	if(a == NULL || X1 == NULL || X2 == NULL) return -1;

	//set seed for Mersene Twister
	init_genrand((unsigned)time(NULL));

	int k = init_k, K, K_list[k_max-init_k+1], i, j, l;
	long t1, t2, DFT_iter_t, FFT_iter_t, DFT_t[k_max-init_k+1], FFT_t[k_max-init_k+1];
	char order[10];

	printf("### Resluts of DFT & FFT ###\n");

	for(i = 0; i <= k_max - init_k; i++){
		K = (int)pow(2, k);
		DFT_iter_t = 0;
		FFT_iter_t = 0;
		for(j = 0; j < ITER; j++){
			//randomly generate a
			for(l = 0; l < K; l++){
				a[l].Re = genrand_res53();
				a[l].Im = 0;
			}

			t1 = get_usec();
			DFT(a, X1, K);
			t2 = get_usec();
			DFT_iter_t += t2 - t1;

			t1 = get_usec();
			FFT(a, X2, k);
			t2 = get_usec();
			FFT_iter_t += t2 - t1;

			//output and compare the results of DFT & FFT
			ord_num(j, order);
			printf("k = %d (%s time)\nDFT\tFFT\n", k, order);
			for(l = 0; l < K; l++){
				if(l >= OUTPUT_LIM) break;
				printf("[%d] ", l);
				if(X1[l].Im < 0){
					printf("%lf%lfi\t", X1[l].Re, X1[l].Im);
				}else{
					printf("%lf+%lfi\t", X1[l].Re, X1[l].Im);
				}
				if(X2[l].Im < 0){
					printf("%lf%lfi\n", X2[l].Re, X2[l].Im);
				}else{
					printf("%lf+%lfi\n", X2[l].Re, X2[l].Im);
				}
			}
			printf("\n");
		}
		K_list[i] = K;
		DFT_t[i] = DFT_iter_t / ITER;
		FFT_t[i] = FFT_iter_t / ITER;

		k += 1;
	}

	//output calculation time
	printf("### Calculation Time ###\n");
	for(i = 0; i <= k_max - init_k; i++){
		printf("K=%d\nDFT:%ld (micor sec)\nFFT:%ld (micro sec)\n\n", K_list[i], DFT_t[i], FFT_t[i]);
	}

	FILE *gp;
	gp = popen("gnuplot -persist","w");
	fprintf(gp, "set logscale\n");

	fprintf(gp, "plot '-' with lp title 'DFT', '-' with lp title 'FFT'\n");
	for(i = 0; i < k_max - init_k; i++){
		fprintf(gp,"%d\t%ld\n", K_list[i], DFT_t[i]);
	}
	fprintf(gp, "e\n");
	for(i = 0; i < k_max - init_k; i++){
		fprintf(gp,"%d\t%ld\n", K_list[i], FFT_t[i]);
	}
	fprintf(gp, "e\n");

	fprintf(gp, "quit\n");
	pclose(gp);

	free(a);
	free(X1);
	free(X2);

	return 0;
}

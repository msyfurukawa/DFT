#include <math.h>
#include "CmpNum.h"

//fast Fourier transform
void FFT(complex *data, complex *X, int k){
	int K = (int)pow(2, (double)k);

	int i, j, a, b;
	//sort data by reversed bit order
	for(i = 0; i < K; i++){
		a = i;
		b = 0;
		for(j = 0; j < k; j++){
			b = b << 1;
			b += a & 1;
			a = a >> 1;
		}
		X[i] = data[b];
	}

	int m, l;
	int p = K / 2;
	complex W_p, tmp_a, tmp_b;
	for(i = 2; i <= K; i *= 2){
		m = K / i;
		for(j = 0; j < m; j++){
			for(l = 0; l < i/2; l++){
				a = i * j + l;
				b = i * j + l + i / 2;
				//W_p is W^(p*l)
				W_p.Re = cos(-2*M_PI*p*l/K);
				W_p.Im = sin(-2*M_PI*p*l/K);
				tmp_a.Re = X[a].Re + X[b].Re * W_p.Re - X[b].Im * W_p.Im;
				tmp_a.Im = X[a].Im + X[b].Im * W_p.Re + X[b].Re * W_p.Im;
				tmp_b.Re = X[a].Re - (X[b].Re * W_p.Re - X[b].Im * W_p.Im);
				tmp_b.Im = X[a].Im - (X[b].Im * W_p.Re + X[b].Re * W_p.Im);
				X[a] = tmp_a;
				X[b] = tmp_b;
			}
		}
		p /= 2;
	}
}

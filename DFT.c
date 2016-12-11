#include <stdlib.h>
#include <math.h>
#include "CmpNum.h"

//discrete Fourier transform according to definition formula
void DFT(complex *data, complex *X, int K){
	complex tmp1, tmp2;
	int k, h;
	double p;
	for(k = 0; k < K; k++){
		tmp1.Re = 0;
		tmp1.Im = 0;
		for(h = 0; h < K; h++){
			p = (double)((k * h) % K);
			tmp2.Re = cos(-2*M_PI*p/K);
			tmp2.Im = sin(-2*M_PI*p/K);
			tmp1.Re += data[h].Re * tmp2.Re - data[h].Im * tmp2.Im;
			tmp1.Im += data[h].Im * tmp2.Re + data[h].Re * tmp2.Im;
		}
		X[k].Re = tmp1.Re;
		X[k].Im = tmp1.Im;
	}
}

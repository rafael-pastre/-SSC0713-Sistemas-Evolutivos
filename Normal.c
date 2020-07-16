#include <stdlib.h>
#include <math.h>
#include <float.h>

double z1;
int generate;

const double epsilon = DBL_MIN;
const double two_pi = 2.0*3.14159265358979323846;
	

// funcao auxiliar
double Normal_aux(double mu, double sigma)
{
	

	generate = !generate;

	if (!generate)
	   return z1 * sigma + mu;

	double u1, u2;
	do
	 {
	   u1 = rand() * (1.0 / RAND_MAX);
	   u2 = rand() * (1.0 / RAND_MAX);
	 }
	while ( u1 <= epsilon );

	double z0;
	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return z0 * sigma + mu;
}

// Essa funcao serve para setar a semente do gerador de numeros
// aleatorios. Por exemplo, chamando sempre reset_normal(1) antes de iniciar a 
// geração de números normais faz a sequencia se repetir
// 
void reset_normal(int seed) {
	srand(seed);
	generate = 0;
}

// esta funcao deve ser chamada seguidamente para gerar uma sequencia
// de valores distribuidos normalmente
double Normal(double mu, double sigma) {
	double d = Normal_aux(mu, sigma);
	return  d < 0.0 ? 0.0 : d;
}

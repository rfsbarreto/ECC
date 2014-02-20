#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ecc_curve.c"

int random_in_range (unsigned int min, unsigned int max)
{
  int base_random = rand(); /* in [0, RAND_MAX] */
  if (RAND_MAX == base_random) return random_in_range(min, max);
  /* now guaranteed to be in [0, RAND_MAX) */
  int range       = max - min,
      remainder   = RAND_MAX % range,
      bucket      = RAND_MAX / range;
  /* There are range buckets, plus one smaller interval
     within remainder of RAND_MAX */
  if (base_random < RAND_MAX - remainder) {
    return min + base_random/bucket;
  } else {
    return random_in_range (min, max);
  }
}

ecc_point* getECCPointFromMessage(char* message){
	int i =7;
	long long x;
//	for (i;i>=0;i--){
//		(*message && 1 << 1)?
//	}
	x = (*message * 72057594037927936) + (*message<<8 * 281474976710656) + (*message<<8 *1099511627776) +(*message<<8 * 4294967296)+
	(*message<<8 * 16777216) + (*message<<8 *65536) + ( *message<<8 * 256) + (*message); 
	return NULL;
}
char* getMessageFromPoint( ecc_point p){

	return NULL;
}

int main(int argc, char** argv){
	FILE* p;
//	char* f=argv[1];
//	printf("%s",a);
	p= fopen(argv[1],"r");
	int prime,a,b,x,order;
	ecc_point* generator,publicKey;
	fscanf(p,"%llu",&prime);
	fscanf(p,"%llu",&a);
	fscanf(p,"%llu",&b);

	//geração das chaves
	int privateKey = random_in_range(0,order);
	ecc_point* publicKey1;
	publicKey1 = mult(*generator,privateKey);

	//encriptação
	char* message = "teste";
	ecc_point* m = getECCPointFromMessage(message);

	int k =  random_in_range(0,order-1);
	ecc_point* c1 = mult(*generator,k);
	ecc_point* c2 = sum(*m,(*mult(publicKey,k)));
	//return (c1,c2)					

	//decriptacao
	char* M= getMessageFromPoint( *sum(*c2,*mult(*c1,privateKey)));

}

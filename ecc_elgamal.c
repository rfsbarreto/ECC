#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ecc_curve.c"

char  *pot_256[20] ={"1", "100", "10000", "1000000", "100000000", "10000000000", "1000000000000", "100000000000000",
 "10000000000000000", "1000000000000000000", "100000000000000000000", "10000000000000000000000", "1000000000000000000000000",
 "100000000000000000000000000","10000000000000000000000000000", "1000000000000000000000000000000", "100000000000000000000000000000000",
 "10000000000000000000000000000000000", "1000000000000000000000000000000000000", "100000000000000000000000000000000000000"};
 /*{"1", "256", "65536", "16777216", "4294967296", "1099511627776", "281474976710656", "72057594037927936", "18446744073709551616",
 "4722366482869645213696", "1208925819614629174706176", "309485009821345068724781056", "79228162514264337593543950336", "20282409603651670423947251286016",
 "5192296858534827628530496329220096", "1329227995784915872903807060280344576", "340282366920938463463374607431768211456", "87112285931760246646623899502532662132736",
 "22300745198530623141535718272648361505980416", "5708990770823839524233143877797980545530986496"};
*/

typedef struct message_point{
	ecc_point* p;
	int qtd_adicoes;
} message_point;

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

message_point* getECCPointFromMessage(char* message){ //,long long primo){
//	int i =7;
	mpz_t x;
	mpz_init(x);
	message_point* m;
	ecc_point* result;
	int i=19;
	for (i;i>=0;i--){
		mpz_t temp;
		mpz_init_set_str(temp,pot_256[i],16);
		mpz_addmul(x,temp,(*message));
		++message;
	}
//	x = (*message * 72057594037927936) + (*message<<8 * 281474976710656) + (*message<<8 *1099511627776) +(*message<<8 * 4294967296)+
//	(*message<<8 * 16777216) + (*message<<8 *65536) + ( *message<<8 * 256) + (*message); 
//Não apliquei o módulo pois o primo escolhido como parametro estoura os 64 bits, desta forma o resultado do módulo sempre seria o próprio valor de x
//	x = x % primo;
//	ecc_point result= isPoint(x);
//	if (result!=NULL){
//		return result;
//	}else{
	i=0;
	do{
		result= existPoint(x);
		i++;
		mpz_add_ui(x,x,1);
		mpz_mod(x,x,prime);
	} while(result!=NULL); 
	if (result){
		m = malloc(sizeof(message_point));
		(*m).p= result;
		(*m).qtd_adicoes=i;
		return m;
	}else
		return NULL;
//	return NULL;
}

char* getMessageFromPoint(message_point* m){
	char* message= malloc(20*sizeof(char));
	int i= 0;
	for (i;i<20;i++){
//		message[i]= (  (*(*m).p).x   << i*8   )  >> (56);
		mpz_sub_ui((*(*m).p).x,(*(*m).p).x,(*m).qtd_adicoes);
		mpz_t aux;
		mpz_init(aux);
		mpz_t pot;
		mpz_init_set_str(pot,pot_256[19-i],16);
		mpz_fdiv_q(aux,(*(*m).p).x,pot);
		message[i]=mpz_get_ui(aux);

	}
	return message;
}

int main(int argc, char** argv){
	FILE* p;
//	char* f=argv[1];
//	printf("%s",a);
	p= fopen(argv[1],"r");

	char* prime_c,a_c,b_c,x_c,order_c;
	ecc_point* generator,publicKey;
	gmp_fscanf(p,"%Zd",prime);
	gmp_fscanf(p,"%Zd",a);
	gmp_fscanf(p,"%Zd",b);

	mpz_t prime,a,b,x,order;
	mpz_init_set_str(prime,prime_c,10);

	//geração das chaves
	int privateKey = random_in_range(0,order);
	ecc_point* publicKey1;
	publicKey1 = mult(*generator,privateKey);

	//encriptação
	char* message = "teste";
	message_point* m = getECCPointFromMessage(message);

//	int k =  random_in_range(0,order-1);
//	ecc_point* c1 = mult(*generator,k);
//	ecc_point* c2 = sum((*(*m).p),(*mult(publicKey,k)));
	//return (c1,c2)					

	//decriptacao
//	ecc_point* aux = mult(*c1,privateKey);
//	(*aux).y=( (*aux).y* -1 ) % prime;
//	message_point *m1 =  malloc( sizeof(message_point));
//	(*m1).p=sum(*c2,/**mult(*c1,privateKey)*/  *aux); 
//	(*m1).qtd_adicoes= (*m).qtd_adicoes;
//	char* M= getMessageFromPoint(m1);

}

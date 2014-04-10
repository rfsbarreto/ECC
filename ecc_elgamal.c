#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ecc_curve.c"

char  *pot_256[21] ={"1", "100", "10000", "1000000", "100000000", "10000000000", "1000000000000", "100000000000000",
 "10000000000000000", "1000000000000000000", "100000000000000000000", "10000000000000000000000", "1000000000000000000000000",
 "100000000000000000000000000","10000000000000000000000000000", "1000000000000000000000000000000", "100000000000000000000000000000000",
 "10000000000000000000000000000000000","1000000000000000000000000000000000000","100000000000000000000000000000000000000","10000000000000000000000000000000000000000"};
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
		mpz_addmul_ui(x,temp,(*message));
		printf(" m:%d       ",(*message));
		++message;
		if (!(*message)){
		//	mpz_mod(x,x,prime);
			break;
		}
	}
	gmp_printf("fase 1 prime:%Zd x= %Zd\n",prime,x);
	
//	x = (*message * 72057594037927936) + (*message<<8 * 281474976710656) + (*message<<8 *1099511627776) +(*message<<8 * 4294967296)+
//	(*message<<8 * 16777216) + (*message<<8 *65536) + ( *message<<8 * 256) + (*message); 
//Não apliquei o módulo pois o primo escolhido como parametro estoura os 64 bits, desta forma o resultado do módulo sempre seria o próprio valor de x
//	x = x % primo;
//	ecc_point result= isPoint(x);
//	if (result!=NULL){
//		return result;
//	}else{
	i=0;
//	mpz_mod(x,x,prime);
//	gmp_printf("tudo bem %Zd \n",prime);
//	scanf("%i",&i);
	do{
		result= existPoint(x);
		i++;
		mpz_add_ui(x,x,1);
	} while(result==NULL);
	mpz_mod((*result).x,(*result).x,prime);
	gmp_printf("fase 2 Ponto: %Zd %Zd \n",(*result).x,(*result).y);
	
	if (result){
		m = malloc(sizeof(message_point));
		(*m).p= result;
		(*m).qtd_adicoes=i-1;
		return m;
	}else
		return NULL;

//	return NULL;
}

char* getMessageFromPoint(message_point* m){
	char* message= malloc(20*sizeof(char));
	int i= 0;
	gmp_printf("X: %Zd ",(*(*m).p).x); 
	for (i;i<20;i++){
		
//		message[i]= (  (*(*m).p).x   << i*8   )  >> (56);
		mpz_t pot;
		mpz_init_set_str(pot,pot_256[20-i],16);
		mpz_sub_ui(pot,pot,1);
		printf(" %Zd \n",pot); 
		mpz_and((*(*m).p).x,(*(*m).p).x,pot);
		mpz_t aux;
		mpz_init(aux);
		mpz_set_str(pot,pot_256[19-i],16);
		mpz_fdiv_q(aux,(*(*m).p).x,pot);
		printf(".%d. ",mpz_get_ui(aux));
		message[i]=mpz_get_ui(aux);

	}
	return message;
}

int main(int argc, char** argv){
	FILE* f;
	char* file=argv[1];
//	printf("%s",a);
	f= fopen(file,"r");

	char prime_c[80],a_c[80],b_c[80],x_c[80],order_c[80],*message=malloc(200*sizeof(char));
	message_point* m;
	ecc_point* generator;
	ecc_point* publicKey;
	ecc_point* p;
	p=malloc(sizeof(ecc_point));
	publicKey=malloc(sizeof(ecc_point));
	generator=malloc(sizeof(ecc_point));
	fscanf(f,"%s \n",prime_c);
	fscanf(f,"%s \n",a_c);
	fscanf(f,"%s \n",b_c);
	mpz_init((*generator).x);
	mpz_init((*generator).y);
	gmp_fscanf(f,"%Zd ",(*generator).x);
	gmp_fscanf(f,"%Zd ",(*generator).y);
	fscanf(f,"%s ",order_c);
	gmp_printf("leituras-> prime:%s, a:%s, b: %s, , order:%s ",prime_c,a_c,b_c,order_c);
/*	mpz_t prime,a,b,x,order;
	mpz_init_set_str(prime,prime_c,16);
	mpz_init_set_str(a,a_c,16);
	mpz_init_set_str(b,b_c,16);
	gmp_printf("leituras-> prime:%Zd, a:%Zd, b: %Zd, ",prime,a,b);
*/	

//	ecc_point* a;
//	a=malloc(sizeof(ecc_point));
//	mpz_init_set_ui((*p).x,0);
//	mpz_init_set_ui((*p).y,1);	
	m = malloc(sizeof(message_point));
	(*m).p=malloc(sizeof(ecc_point));
	(*m).p=p;
	(*m).qtd_adicoes=0;
	
	init_curve(a_c,b_c,prime_c,order_c,1,*generator);
	//geração das chaves
	mpz_t random;
	mpz_init(random); 
	gmp_randstate_t st;
	gmp_randinit_default(st);
	gmp_randseed_ui(st,time(NULL));
	mpz_urandomm(random, st, order);

	mpz_t  privateKey;
	mpz_init_set(privateKey,random); // = random_in_range(0,order);
	ecc_point* publicKey1;
	publicKey1 = mult(*generator,privateKey);

	//encriptação
	printf("mensagem: ");
	scanf("%s",message);// ="8aa";// getMessageFromPoint(m);

//	printf("Mensagem: %s",message);
	
	m= getECCPointFromMessage(message);
	if (!m){
		printf("ERROR \n");
		return -1;
	}
		char* msg;
		gmp_printf("x,y,a:  %Zd %Zd %d \n",(*(*m).p).x,(*(*m).p).y,(*m).qtd_adicoes);
		msg= getMessageFromPoint(m);
	
	gmp_printf("Mensagem:  %s \n", msg);
	
	gmp_randseed_ui(st,time(NULL));
	mpz_urandomm(random, st, order);
/*	gmp_printf(" random: %Zd %Zd",random,order);
	gmp_randclear(st);
*/	
//	int k =  random_in_range(0,order-1);
	ecc_point* c1 = mult(*generator,random);
	ecc_point* c2 = sum((*(*m).p) ,(*mult(*publicKey,random)));
	//return (c1,c2)					
	printf(" Fim encriptacao\n");
	
	//decriptacao
	ecc_point* aux = mult(*c1,privateKey);
	mpz_neg((*aux).y,(*aux).y);
//	(*aux).y=( (*aux).y* -1 ) % prime;
	message_point *m1 =  malloc( sizeof(message_point));
	(*m1).p= malloc(sizeof(ecc_point));

	(*m1).p=sum(*c2,/**mult(*c1,privateKey)*/  *aux); 
	(*m1).qtd_adicoes= (*m).qtd_adicoes;
	char* M;
	printf("%s \n",getMessageFromPoint(m1));
	printf("Mensagem final: %s \n",M);
}


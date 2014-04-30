#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ecc_curve.c"

char  *pot_256[21] ={"1", "100", "10000", "1000000", "100000000", "10000000000", "1000000000000", "100000000000000",
 "10000000000000000", "1000000000000000000", "100000000000000000000", "10000000000000000000000", "1000000000000000000000000",
 "100000000000000000000000000","10000000000000000000000000000", "1000000000000000000000000000000", "100000000000000000000000000000000",
 "10000000000000000000000000000000000","1000000000000000000000000000000000000","100000000000000000000000000000000000000","10000000000000000000000000000000000000000"};
int MSG_BYTES_MAX=2;
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
	int i=MSG_BYTES_MAX-1;  //19;
	for (i;i>=0;i--){
		mpz_t temp;
		mpz_init_set_str(temp,pot_256[i],16);
		gmp_printf("temp: %Zd ",temp);
		mpz_addmul_ui(x,temp,(*message));
		printf(" m:%d \n",(*message));
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
	char* message= malloc(MSG_BYTES_MAX*sizeof(char));
	int i=MSG_BYTES_MAX ;
	gmp_printf("X: %Zd ",(*(*m).p).x); 
	mpz_sub_ui((*(*m).p).x,(*(*m).p).x,(*m).qtd_adicoes);
/*	for (i;i>=0;i--){
		mpz_t temp;
		mpz_init_set_str(temp,pot_256[i],16);
		gmp_printf("temp: %Zd ",temp);
//		mpz_addmul_ui(x,temp,(*message));
//		printf(" m:%d \n",(*message));
//		++message;
//		if (!(*message)){
		//	mpz_mod(x,x,prime);
//			break;
//		}
	}*/
		printf("\n\n");
	i=0;
	for (i;i<MSG_BYTES_MAX;i++){
		mpz_t pot;
//		mpz_init(pot1);
//		message[i]= (  (*(*m).p).x   << i*8   )  >> (56);
		mpz_init_set_str(pot,pot_256[MSG_BYTES_MAX-i],16);
		mpz_sub_ui(pot,pot,1);
		gmp_printf("i:%d pot: %Zd \n",i,pot); 
		mpz_and((*(*m).p).x,(*(*m).p).x,pot);
		gmp_printf("and_X: %Zd ",(*(*m).p).x);
		mpz_t aux;
		mpz_init(aux);
		mpz_set_str(pot,pot_256[MSG_BYTES_MAX-1-i],16);
		mpz_fdiv_q(aux,(*(*m).p).x,pot);
		printf(".%d.%c ",mpz_get_ui(aux),mpz_get_ui(aux));
		message[i]=(mpz_get_ui(aux)>=20 && mpz_get_ui(aux)<127)?mpz_get_ui(aux):'\0';

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
	scanf("%[0-9a-zA-Z ]s",message);// ="8aa";// getMessageFromPoint(m);

//	printf("Mensagem: %s",message);
	
	m= getECCPointFromMessage(message);
	if (!m){
		printf("ERROR \n");
		return -1;
	}
		char* msg;
		gmp_printf("x,y,a:  %Zd %Zd %d \n",(*(*m).p).x,(*(*m).p).y,(*m).qtd_adicoes);
		msg= getMessageFromPoint(m);
	
	gmp_printf("Mensagem:  %c \n", msg[1]);
	
	gmp_randseed_ui(st,time(NULL));
	mpz_urandomm(random, st, order);
	gmp_printf(" random: %Zd %Zd",random,order);
/*	gmp_randclear(st);
*/	
//	int k =  random_in_range(0,order-1);
	ecc_point* c1 = mult(generator_point,random);
	ecc_point* aux=mult(*publicKey1,random);
	gmp_printf("aux.x %Zd aux.y %Zd \n",(*c1).x,(*c1).y);
	gmp_printf("C1.x %Zd C1.y %Zd\n",(*c1).x,(*c1).y);

	//if (!aux)
	//	printf("aa\n");
//	sum((*(*m).p),(*(*m).p));
	ecc_point* c2 = sum((*(*m).p) ,(*aux));
//	gmp_printf("C1.x %Zd C1.y %Zd\n",(*c1).x,(*c1).y);

	gmp_printf("C2.x %Zd C2.y %Zd\n",(*c2).x,(*c2).y);

	//return (c1,c2)					
	printf(" Fim encriptacao\n");
	
	//decriptacao
	aux = mult(*c1,privateKey);
	gmp_printf("priv: %Zd aux.x %Zd aux.y %Zd\n",privateKey,(*aux).x,(*aux).y);
	mpz_neg((*aux).y,(*aux).y);
//	gmp_printf(
//	(*aux).y=( (*aux).y* -1 ) % prime;
	message_point *m1 =  malloc( sizeof(message_point));
//	(*m1).p= malloc(sizeof(ecc_point));

	(*m1).p=sum(*c2,/**mult(*c1,privateKey)*/  *aux); 
	(*m1).qtd_adicoes= (*m).qtd_adicoes;
	gmp_printf("M1.x: %Zd M1.y: %Zd",(*(*m1).p).x,(*(*m1).p).y);
	char* M;
	printf(" tada: %s \n",getMessageFromPoint(m1));
//	printf("Mensagem final: %s \n",M);

}



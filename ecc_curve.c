#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ecc_curve.h"

#define BASE_16 16

//mpz_t a,b,prime,order;
//int cofactor;
//ecc_point generator_point;


/*
int modularinverse( int aux, int prime){
	int c=b/a,x=0;
	do{
	printf(" %d %d |",aux,prime);
		if((aux>prime)||(aux==1)) x=1;
		if((c*aux)%prime==1) x=c;
		else c++;
	} while(x==0);
	printf("invmd: %d \n",x);
	return x;
}
*/
int init_curve(char* a1, char* b1,char* prime1, char* order1,int cofactor1,ecc_point g1){
//	a=a1;
	mpz_init_set_str(a,a1,BASE_16);
	mpz_init_set_str(b,b1,BASE_16);
	mpz_init_set_str(prime,prime1,BASE_16);
	mpz_init_set_str(order,order1,BASE_16);

	//printf("%d\n ",b);
	//b=b1;
	//printf("%d\n ",b);
/*	prime=prime1;
	generator_point = g1;
	n= order1;*/
	cofactor= cofactor1;
	if ( existPoint1(g1.x,g1.y))
		return 0;
	else
		return -1;
}

int existPoint1(mpz_t x, mpz_t  y){
	mpz_t exp,eq_result;
	mpz_init(eq_result);  //Equation Result
	mpz_init(exp); //Exponentiation Result
	mpz_pow_ui(exp,x,3);
	mpz_addmul(exp,x,a);
//	mpz_add(eq_result,eq_result,exp);
	mpz_add(exp,exp,b);	
//	long long soma = pow(x,3)+ a*x + b;
	mpz_mod(exp,exp,prime);
	if (mpz_perfect_square_p(exp)){
		mpz_sqrt(eq_result,x);
		if (mpz_cmp(y,eq_result)==0)
			return 1;
	}else
		return 0;	
//	long long resultado= sqrt( ((int) soma)%prime );  
//	printf("%llu %lld %f \n",l,x,sqrt(pow(x,3)+ a*x + b));
	
//	return y== resultado || y==prime - resultado   ;
}

ecc_point* sum(ecc_point p1,ecc_point p2){
	ecc_point* result;
//	printf("primo: %d a:%d b:%d\n\n",prime,a,b);
	result = malloc(sizeof(ecc_point));
	mpz_init((*result).x);
	mpz_init((*result).y);
	if (mpz_cmp(p1.x,p2.x)==0 && mpz_cmp(p1.y,p2.y)==0)
		result=NULL;//mult(p1,2);
	else
		if( mpz_cmp(p1.x,p2.x)==0 && mpz_cmpabs(p2.y,p1.y)==0)
			result=INFINITY_POINT;
		else{
			mpz_t delta_x,x,y,delta_y,s,s_2;
			mpz_init(delta_x);
			mpz_init(x); mpz_init(y);
			mpz_init(s); mpz_init(s_2);
			mpz_init(delta_y);
			mpz_sub(delta_x,p1.x,p2.x);
	//		gmp_printf("Deltax: %Zd \n",delta_x);
			mpz_sub(delta_y,p1.y,p2.y);
	//		gmp_printf("Deltay: %Zd \n",delta_y);
			mpz_mod(delta_x,delta_x,prime);
	//		gmp_printf("Deltax/prime: %Zd \n",delta_x);
			mpz_invert(delta_x,delta_x,prime);
		//	gmp_printf("Modulo_inv: %Zd \n",delta_x);
			mpz_mul(s,delta_x,delta_y);
			mpz_mod(s,s,prime);
		//	gmp_printf("S: %Zd \n",s);
			mpz_pow_ui(s_2,s,2);
			//gmp_printf("S^2: %Zd \n",s_2);
			mpz_sub(x,s_2,p1.x);
		//	gmp_printf("%Zd \n",delta_x);
			mpz_sub(x,x,p2.x);
			mpz_mod(x,x,prime);
//			long long x= (( s*s - p1.x - p2.x))%prime;
			mpz_set((*result).x,x);
//			printf("s:%d prime:%d x:%lld result.x:%lld \n",s,prime,x,(*result).x);
		//	gmp_printf("%Zd \n",x);
			mpz_sub(delta_x,p2.x,x);
			mpz_neg(y,p2.y);
			mpz_addmul(y,s,delta_x);
			mpz_mod(y,y,prime);
			//gmp_printf("%Zd \n",y);

		//	long long y=(( -p2.y + s*(p2.x-(*result).x)))%prime;	
			mpz_set((*result).y,y);
		};
	return result;	
}

ecc_point* double_p(ecc_point p){
	ecc_point* result;
	result= malloc(sizeof(ecc_point));
	mpz_init((*result).x);
	mpz_init((*result).y);
	printf("save point");
	if (mpz_cmp_ui(p.y,0)!=0){
		mpz_t s,d_y,d_x,y;
		mpz_init(d_y);
		mpz_init(s);
		mpz_init(y);
		mpz_init(d_x);
		mpz_pow_ui(s,p.x,2);
//		gmp_printf("s1:%Zd \n",s);
		mpz_mul_si(s,s,3);
//		gmp_printf("s2:%Zd \n",s);
		mpz_add(s,s,a);
//		gmp_printf("s3:%Zd \n",s);
		mpz_mul_si(d_y,p.y,2);
		mpz_mod(d_y,d_y,prime);
		mpz_invert(d_y,d_y,prime);
//		gmp_printf(" %Zd \n",d_y);
		mpz_mul(s,s,d_y);
		mpz_mod(s,s,prime);	
//		gmp_printf(" s4: %Zd",s);	
		mpz_mul_ui(d_x,p.x,2);
		mpz_pow_ui((*result).x,s,2);
//		gmp_printf("r.x: %Zd \n",(*result).x);

	//	mpz_mod((*result).x,(*result).x,prime);
		mpz_sub((*result).x,(*result).x,d_x);
		mpz_mod((*result).x,(*result).x,prime);
//		gmp_printf(" x: %Zd ",(*result).x);
		mpz_neg((*result).y,p.y);
		mpz_sub(d_x,p.x,(*result).x);
		mpz_mul(s,s,d_x);
		mpz_add((*result).y,(*result).y,s);
		mpz_mod((*result).y,(*result).y,prime);
//		gmp_printf(" y: %Zd \n",(*result).y);

	}else
		result=INFINITY_POINT;
	return result;
}

ecc_point* mult(ecc_point p, int value){
	ecc_point* result ;
	result =malloc(sizeof(ecc_point));
//	result=p;
	if (value==0)
		return NULL;
	if (value==1)
		return (&p);
	int aux=value;
	if (aux!=0){
	//	ecc_point p1= mult(result,
		if (aux%2 != 0 )
			result = sum(p, (*mult(p,aux-1)));
		else
			result = double_p((*mult(p,aux/2)));
	}
	return result;

} 

ecc_point* existPoint(mpz_t  p){
//	return (p.y== sqrt(pow(p.x,3) + a*p.x + b));
	//printf("%d\n",b);
	mpz_t l;
	mpz_init(l);
	mpz_pow_ui(l,p,3);
	mpz_addmul(l,a,p);
	mpz_add(l,l,b);
	mpz_mod(l,l,prime);
	mpz_t i;
	mpz_init_set_ui(i,0);
	while(mpz_cmp(i,l)!=0){
		mpz_t y;
		mpz_init_set(y,i);
		mpz_pow_ui(y,y,2);
		mpz_mod(y,y,prime);
		if (mpz_cmp(y,l)==0){
			ecc_point* r= malloc(sizeof(ecc_point));
			mpz_init_set((*r).x,l);
			mpz_init_set((*r).y,y);
		}else
			mpz_add_ui(i,i,1);
	}
	return NULL;
		
//	if (!mpz_perfect_square_p(l)){
//		return NULL;
	}
/*else{
		mpz
		mpz_sqrt(l,l);
		mpz_= sqrt(pow(p,3)+ a*p + b);
		printf("%llu %d %f \n",l,b,sqrt(pow(p,3)+ a*p + b));
		if (round(l)==l){
			ecc_point* result= malloc( sizeof(ecc_point));
			(*result).x=p;
			(*result).y=l;		
			return result;;
		}else
			return NULL;
	}
}*/
/*
ecc_point* existPoint(long long  p){
//	return (p.y== sqrt(pow(p.x,3) + a*p.x + b));
	//printf("%d\n",b);
	long long l = sqrt(pow(p,3)+ a*p + b);
	printf("%llu %d %f \n",l,b,sqrt(pow(p,3)+ a*p + b));
	if (round(l)==l){
		ecc_point* result= malloc( sizeof(ecc_point));
		(*result).x=p;
		(*result).y=l;		
		return result;;
	}else
		return NULL;
}




ecc_point* double_p(ecc_point p){
	ecc_point* result;
	result= malloc(sizeof(ecc_point));
	if (p.y!=0){
		int s= (3*p.x*p.x + a)/( 2*p.y);
		(*result).x=s*s -2* p.x;
		(*result).y=-p.y + s*(p.x-(*result).x);

	}else
		result=INFINITY_POINT;
	return result;
}

ecc_point* mult(ecc_point p, int value){
	ecc_point* result ;
	result =malloc(sizeof(ecc_point));
//	result=p;
	int aux=value;
	if (aux!=0){
	//	ecc_point p1= mult(result,
		if (aux%2 != 0 )
			result = sum(p, (*mult(p,aux-1)));
		else
			result = double_p((*mult(p,aux/2)));
	}
	return result;

} */

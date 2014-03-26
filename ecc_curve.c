#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ecc_curve.h"
#include<gmp.h>

int a,b=5,prime,order,cofactor;
ecc_point generator_point;



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

int init_curve(int a1, int b1,int prime1, int order1,int cofactor1,ecc_point g1){
	a=a1;
	//printf("%d\n ",b);
	b=b1;
	//printf("%d\n ",b);
	prime=prime1;
	generator_point = g1;
	n= order1;
	h= cofactor1;
	if ( existPoint1(g1.x,g1.y))
		return 0;
	else
		return -1;
}


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

int existPoint1(long long x, long long y){
	long long soma = pow(x,3)+ a*x + b;
	long long resultado= sqrt( ((int) soma)%prime );  
//	printf("%llu %lld %f \n",l,x,sqrt(pow(x,3)+ a*x + b));
	
	return y== resultado || y==prime - resultado   ;
}



ecc_point* sum(ecc_point p1,ecc_point p2){
	ecc_point* result;
//	printf("primo: %d a:%d b:%d\n\n",prime,a,b);
	result = malloc(sizeof(ecc_point));
	if ((p1.x==p2.x) && (p1.y==p2.y))
		result=mult(p1,2);
	else
		if( (p1.x==p2.x)&&(p2.y==-p1.y))
			result=INFINITY_POINT;
		else{
			int delta_x=(p1.x-p2.x);
			int s= ((p1.y-p2.y)* modularinverse((delta_x>=0?delta_x:prime+delta_x),prime))%prime;
			long long x= (( s*s - p1.x - p2.x))%prime;
			(*result).x= x>=0?x:prime+x;
//			printf("s:%d prime:%d x:%lld result.x:%lld \n",s,prime,x,(*result).x);
			long long y=(( -p2.y + s*(p2.x-(*result).x)))%prime;	
			(*result).y= y>=0?y:prime+y;
		};
	return result;	
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

}

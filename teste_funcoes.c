#include<stdio.h>
#include "ecc_curve.c"




int main(){
//	printf("%d %d", (-3)%(5U),(int) floor());
	ecc_point* p=malloc(sizeof(ecc_point));
	ecc_point* p2=malloc(sizeof(ecc_point));
	gmp_scanf("%Zd %Zd \n",&(*p).x,&(*p).y);
	gmp_scanf("%Zd %Zd: \n",&(*p2).x,&(*p2).y);
	gmp_printf("%Zd \n",(*p).x);
	int a=init_curve("2","4","3d","57",1,*p);
	ecc_point* r= sum(*p,*p2);
//	printf("Resultado: %Zd %Zd\n",(*r).x,(*r).y);
	

}

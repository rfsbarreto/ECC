#include<stdio.h>
//#include "ecc_curve.c"
#include "ecc_elgamal.c"



int main(){
//	printf("%d %d", (-3)%(5U),(int) floor());
	ecc_point* p=malloc(sizeof(ecc_point));
	ecc_point* p2=malloc(sizeof(ecc_point));
	gmp_scanf("%Zd %Zd \n",&(*p).x,&(*p).y);
	gmp_scanf("%Zd %Zd: \n",&(*p2).x,&(*p2).y);
//	gmp_printf("%Zd \n",(*p).x);
	int a=init_curve("2","4","3d","57",1,*p);
	ecc_point* r= sum(*p,*p2);
	gmp_printf("Soma: %Zd %Zd\n",(*r).x,(*r).y);
	p2= double_p(*p);
	gmp_printf(" Dobro: %Zd %Zd \n",(*p2).x,(*p2).y);	
	p2=mult((*p),4);
	gmp_printf(" 3x: %Zd %Zd \n",(*p2).x,(*p2).y);	

}

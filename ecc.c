typedef struct ecc_point{
	int x,y;
} ecc_point;


int main(){}


ecc_point sum(ecc_point p1,ecc_point p2){
	ecc_point result = malloc(sizeof(ecc_point));
	if ((p1.x==p2.x) && (p1.y==p2.y))
		result=mult(p1,2);
	else
		if( (p1.x==p2.x)&&(p2.y==-p1.y)
			result=INFINITY_POINT
		else{
			int s= (p1.y-p2.y)/(p1.x-p2.x)
			result.x= s*s - p1.x - p2.x
			result.y= - p1.x+ s(p1.x-p2.x)	
		}
	return result;	
}

ecc_point mult(ecc_point p, int value){
	ecc_point result = malloc(sizeof(ecc_point));
	result=p;
	int aux=value;
	while (aux!=0){
	//	ecc_point p1= mult(result,
		if (aux%2 != 0 )
			result = sum(p, mult(p,aux/2));
		else
			result = double_p(mult(p,aux/2);
	}

}


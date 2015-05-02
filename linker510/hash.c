#include <stdlib.h>
#include <stdio.h>


int hashj(int x,int cont){
     int aleatorio;
     aleatorio= ((x*cont+71)*23209)%199;
     if(aleatorio <= 0 || aleatorio == 199 || aleatorio >=200){
         aleatorio=hashj(x+3,cont+13);
         return aleatorio;
    }
    else{
     return aleatorio;
    }
}
int main()
{
	int cont =27;
	int i=0;
	while(1){
		++i;
	 	printf("n = %i \n",hashj(i,cont));
	 	cont++;
	}
	return 0;	
}
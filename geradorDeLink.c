#include<stdio.h>
#include<stdlib.h>

int main (int argc, char * argv[]){
	char aux;
	FILE * fp;
	int lineCounter = 1, lineCounterInicial = 594, lineCounterFinal = 10596, flag = 0;
	fp = fopen("index.html", "r");
	
	while(!feof(fp)){
		aux = fgetc(fp);
		if(aux == '\n'){
			lineCounter ++;
		}
		 if(lineCounter > lineCounterInicial && !((lineCounter - lineCounterInicial) % 40) && lineCounter < lineCounterFinal){
			
			printf("%c", aux);
		}
	}
	printf("\n");
	fclose(fp);
//	printf("LINE COUNTER :::: ___>%d\n", lineCounter);
	return 0;
}

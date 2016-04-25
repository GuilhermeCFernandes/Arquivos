#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define TERMINADOR_REGISTRO *
#define TERMINADOR_CAMPO +

typedef struct registro{

	uint32_t id;
	char * titulo;
	char * descricao;
	char * producao;
	uint16_t lancamento;
	uint8_t temporada;
	char * genero;
	
}REGISTRO;

REGISTRO * leRegistro (FILE * pf){
	REGISTRO * registro = NULL;
	return registro;
}

int escreveRegistro (FILE * fp){
	return 0;
}

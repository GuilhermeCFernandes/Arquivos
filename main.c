#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct registro {
    int id;
    char *titulo;
    char *descricao;
    char *producao;
    int ano;
    int temporada;
    char *genero;

} REGISTRO;

void generateBD (void);


int main (int argc, char *argv[]) {

    generateBD ();

    return 0;
}

void generateBD (void) {
//Generate random vector with 100 pos
    int i, j;
    int numb;
    int size;

    int randVector[100];
    void *regVector = NULL;

    char terminadorCampo = '|';
    char terminadorRegistro = '#';

    REGISTRO *registros = NULL;
    registros = (REGISTRO *) malloc (250 * sizeof(REGISTRO *));

    FILE *titulo = NULL;
    FILE *descricao = NULL;
    FILE *producao = NULL;
    FILE *ano = NULL;
    FILE *temporada = NULL;
    FILE *genero = NULL;
    FILE *BancoDeDados = NULL;


    srand(time(NULL));
    for (i = 0; i < 100; i++) {

        numb = rand() % 100;

        for (j = 0; j < i; j++) if (numb == randVector[j]) break;
        if (j < i) {
                randVector[i] = numb;
                printf("%d ", numb);
        }
        else i++;
    }

    titulo = fopen ("nomes.txt", "r");
    descricao = fopen ("desc.txt", "r");
    producao = fopen ("paises.txt", "r");
    ano = fopen ("anos.txt", "r");
    temporada = fopen ("temporadas.txt", "r");
    genero = fopen ("generos.txt", "r");


    for (i = 0; i < 250; i++) {
        registros[i].titulo = NULL;
        registros[i].descricao = NULL;
        registros[i].producao = NULL;
        registros[i].genero = NULL;

        j = 0;
        getline (&registros[i].titulo, &j, titulo);

        j = 0;
        getline (&registros[i].descricao, &j, descricao);

        j = 0;
        getline (&registros[i].producao, &j, producao);

        fscanf  (ano, "%d", &registros[i].ano);
        fscanf  (temporada, "%d", &registros[i].temporada);

        j = 0;
        getline (&registros[i].genero, &j, genero);
    }


    //Salvando no arquivo
    BancoDeDados = fopen ("BancoDeDados", "w");
    for (i = 0; i < 100; i++) {
        size = sizeof(char) * (strlen(registros[i].titulo) + strlen(registros[i].descricao) + strlen(registros[i].producao) + strlen (registros[i].genero));
        size += sizeof(int) * 3;
        size += sizeof(char) * 8;

        regVector = malloc (size);
        size = 0;

        memcpy (regVector, &i, sizeof(int));
        size += sizeof(int);

        memcpy (regVector, &terminadorCampo, sizeof(char));
        size += sizeof(char);

        memcpy(regVector + size, registros[i].titulo, strlen(registros[i].titulo) * sizeof(char));
        size += strlen(registros[i].titulo) * sizeof(char);

        memcpy (regVector, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy(regVector + size, registros[i].descricao, strlen(registros[i].descricao) * sizeof(char));
        size += strlen(registros[i].descricao) * sizeof(char);

        memcpy (regVector, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy(regVector + size, registros[i].producao, strlen(registros[i].producao) * sizeof(char));
        size += strlen(registros[i].producao) * sizeof(char);

        memcpy (regVector, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy (regVector, &registros[i].ano, 1 * sizeof(int));
        size += sizeof(int);

        memcpy (regVector, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy (regVector, &registros[i].temporada, 1 * sizeof(int));
        size += sizeof(int);

        memcpy (regVector, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy(regVector + size, registros[i].genero, strlen(registros[i].genero) * sizeof(char));
        size += strlen(registros[i].genero) * sizeof(char);

        memcpy (regVector, &terminadorCampo, sizeof(char));
        size += sizeof(char);
        memcpy (regVector, &terminadorRegistro, sizeof(char));
        size += sizeof(char);

        fwrite (regVector, 1, size, BancoDeDados);
    }

}

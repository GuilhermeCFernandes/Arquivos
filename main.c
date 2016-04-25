#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define TERMINADOR_CAMPO '|'
#define TERMINADOR_REGISTRO '#'

typedef struct registro {
    int id;
    char *titulo;
    char *descricao;
    char *producao;
    int ano;
    int temporada;
    char *genero;
} REGISTRO;

enum opcoes {
    SAIR,
    BUSCAR,
    DUMP,
    ALEATORIZAR
};

void generateBD (void);
void dumpAll(FILE *);
void dumpData (FILE *);
void dumpReg (void);
long find(FILE *, int);

int main (int argc, char *argv[]) {
    FILE *f = fopen("BancoDeDados", "r");
    int id, op;
    long offset;
    generateBD ();

    do {
        rewind(f);
        printf("1) Buscar\n2) Dump\n3) Aleatorizar\n0) Sair\n");
        scanf("%d", &op);
        switch (op) {
            case BUSCAR:
                printf("Digite o id a ser buscado: ");
                scanf("%d", &id);
                offset = find(f, id);
                if (offset == EOF)
                    printf("ID não encontrado.\n");
                else {
                    fseek(f, offset, SEEK_SET);
                    dumpData(f);
                }
                break;

            case DUMP:
                dumpAll(f);
                break;

            case ALEATORIZAR:
                fclose(f);
                generateBD();
                f = fopen("BancoDeDados", "r");
                break;
        }
    } while (op != SAIR);

    fclose(f);

    return 0;
}

void ignoreStr(FILE *stream) {
    char c;

    do {
        c = fgetc(stream);
    } while (!feof(stream) && c != TERMINADOR_CAMPO);
}

void ignoreInt(FILE *stream) {
    int x;
    fread(&x, sizeof(int), 1, stream);
    fgetc(stream);
}

long find(FILE *stream, int id) {
    int idReg;

    do {
        fread(&idReg, sizeof(int), 1, stream);
        fgetc(stream);
        if (idReg == id)
            return ftell(stream) - sizeof(int) - sizeof(char);

        ignoreStr(stream);
        ignoreStr(stream);
        ignoreStr(stream);
        ignoreInt(stream);
        ignoreInt(stream);
        ignoreStr(stream);
        fgetc(stream);
    } while (!feof(stream));

    return EOF;
}

void dumpStr(FILE *stream) {
    char c;

    do {
        c = fgetc(stream);
        if (c != TERMINADOR_CAMPO)
            fputc(c, stdout);
    } while (!feof(stream) && c != TERMINADOR_CAMPO);
}

void dumpInt(FILE *stream) {
    int x;
    fread(&x, sizeof(int), 1, stream);
    fgetc(stream);
    printf("%d", x);
}

void dumpAll(FILE *stream) {
    int i;
    for (i = 0; i < 100; i++)
        dumpData(stream);
}

void printCampo(char *nome, void (*f) (FILE *), FILE *stream) {
    printf("%s\t", nome);
    f(stream);
    printf("\n");
}

void dumpData(FILE *stream) {
    printCampo("id\t", &dumpInt, stream);
    printCampo("tituloSerie", &dumpStr, stream);
    printCampo("descSerie", &dumpStr, stream);
    printCampo("producao", &dumpStr, stream);
    printCampo("anoLançamento", &dumpInt, stream);
    printCampo("temporada", &dumpInt, stream);
    printCampo("generoSerie", &dumpStr, stream);
    fgetc(stream);
    printf("================\n");
}

char *meuGetLine(size_t *b, FILE *stream) {
    char *s = NULL;
    int i;
    getline(&s, b, stream);
    i = strlen(s)-1;

    if (i >= 0 && s[i] == '\n')
        s[i] = '\0';

    return s;
}

int nextRandId(bool *ids) {
    int id = -1;

    do {
        id = rand() % 100;
        if (ids[id])
            id = -1;
        else
            ids[id] = 1;
    } while (id < 0);

    return id;
}

void generateBD (void) {
//Generate random vector with 100 pos
    size_t i, j;
    int id;
    int numb;
    int size;

    int randVector[100];
    bool *randIds = calloc(100, sizeof(bool));
    void *regVector = NULL;

    char terminadorCampo = TERMINADOR_CAMPO;
    char terminadorRegistro = TERMINADOR_REGISTRO;

    REGISTRO *registros = NULL;
    registros = (REGISTRO *) malloc (250 * sizeof(REGISTRO));

    FILE *titulo = NULL;
    FILE *descricao = NULL;
    FILE *producao = NULL;
    FILE *ano = NULL;
    FILE *temporada = NULL;
    FILE *genero = NULL;
    FILE *BancoDeDados = NULL;


    srand(time(NULL));
    for (i = 0; i < 100; i++) {
        numb = rand() % 250;

        for (j = 0; j < i; j++)
            if (numb == randVector[j]) break;

        if (j == i) {
            randVector[i] = numb;
        } else
            i--;
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
        registros[i].titulo = meuGetLine (&j, titulo);

        j = 0;
        registros[i].descricao = meuGetLine (&j, descricao);

        j = 0;
        registros[i].producao = meuGetLine (&j, producao);

        fscanf  (ano, "%d", &registros[i].ano);
        fscanf  (temporada, "%d", &registros[i].temporada);

        j = 0;
        registros[i].genero = meuGetLine (&j, genero);
    }


    //Salvando no arquivo
    BancoDeDados = fopen ("BancoDeDados", "w");
    for (j = 0; j < 100; j++) {
        i = randVector[j];
        id = nextRandId(randIds);
        size = sizeof(char) * (strlen(registros[i].titulo) + strlen(registros[i].descricao) + strlen(registros[i].producao) + strlen (registros[i].genero));
        size += sizeof(int) * 3;
        size += sizeof(char) * 8;

        regVector = calloc(size, 1);
        size = 0;

        memcpy (regVector + size, &id, sizeof(int));
        size += sizeof(int);

        memcpy (regVector + size, &terminadorCampo, sizeof(char));
        size += sizeof(char);

        memcpy(regVector + size, registros[i].titulo, strlen(registros[i].titulo) * sizeof(char));
        size += strlen(registros[i].titulo) * sizeof(char);

        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy(regVector + size, registros[i].descricao, strlen(registros[i].descricao) * sizeof(char));
        size += strlen(registros[i].descricao) * sizeof(char);

        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy(regVector + size, registros[i].producao, strlen(registros[i].producao) * sizeof(char));
        size += strlen(registros[i].producao) * sizeof(char);

        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy (regVector + size, &registros[i].ano, 1 * sizeof(int));
        size += sizeof(int);

        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy (regVector + size, &registros[i].temporada, 1 * sizeof(int));
        size += sizeof(int);

        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        memcpy(regVector + size, registros[i].genero, strlen(registros[i].genero) * sizeof(char));
        size += strlen(registros[i].genero) * sizeof(char);

        memcpy (regVector + size, &terminadorCampo, sizeof(char));
        size += sizeof(char);
        memcpy (regVector + size, &terminadorRegistro, sizeof(char));
        size += sizeof(char);

        fwrite (regVector, 1, size, BancoDeDados);
        free(regVector);
    }
    fclose (titulo);
    fclose (descricao);
    fclose (producao);
    fclose (ano);
    fclose (temporada);
    fclose (genero);
    fclose(BancoDeDados);

    for (i = 0; i < 250; i++) {
        free(registros[i].titulo);
        free(registros[i].descricao);
        free(registros[i].genero);
        free(registros[i].producao);
    }
    free(registros);
    free(randIds);

}

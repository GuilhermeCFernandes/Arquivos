#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define TERMINADOR_CAMPO '|'
#define TERMINADOR_REGISTRO '#'

/*Estrutura usada para armazenar registros em RAM antes da criação do arquivo binario*/
typedef struct registro {
    int id;
    char *titulo;
    char *descricao;
    char *producao;
    int ano;
    int temporada;
    char *genero;
} REGISTRO;

/*Opções do menu*/
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
    
	/*Variaveis*/
    FILE *f; //Arquivo de dados
    int id; //Chave de busca 
    int op; // Oberador
    long offset; //byte offset
    
    f = fopen("BancoDeDados", "a");
    fclose(f);
    f = fopen("BancoDeDados", "r");
    generateBD (); //gera o arquivo de dados a partir dos arquivos texto

    // inicio do programa
    do {

        rewind(f);
        printf("1) Buscar\n2) Dump\n3) Aleatorizar\n0) Sair\n"); //Imprime interface
        scanf("%d", &op); //Busca pela opção do usuario
        
        //Executa a opção desejada
        switch (op) {

            case BUSCAR:
            	//Recebe o ID
                printf("Digite o id a ser buscado: ");
                scanf("%d", &id);

                //Encontra o byte offset do campo, caso encontrado
                offset = find(f, id);

                //Imprime o campo/mensagem de erro
                if (offset == EOF)
                    printf("ID não encontrado.\n");
                else {
                    fseek(f, offset, SEEK_SET);
                    dumpData(f);
                }
                break;

            case DUMP:
            	//imprime todos os registros na tela
                dumpAll(f);
                break;

            case ALEATORIZAR:

            	//Gera um novo arquivo de banco de dados
                fclose(f);
                generateBD();
                f = fopen("BancoDeDados", "r");
                break;
        }
    } while (op != SAIR);

    fclose(f);

    return 0;
}

/* ignoreStr() - Busca pelo terminador de campo em um campo de tamanho variavel que armazena uma string
 *	Parameter:
 *		FILE *stream: file stream
 *	Return:
 *		-		
*/
void ignoreStr(FILE *stream) {
    char c;

    do {
        c = fgetc(stream);
    } while (!feof(stream) && c != TERMINADOR_CAMPO);
}

/* ignoreStr() - Busca pelo terminador de campo em um campo de tamanho fixo que armazena um inteiro
 *	Parameter:
 *		FILE *stream: file stream
 *	Return:
 *		-		
*/
void ignoreInt(FILE *stream) {
    int x;
    fread(&x, sizeof(int), 1, stream);
    fgetc(stream);
}


/* find () - Executa a busca sequencial no arquivo de usando como chave primaria o ID
*	Parameter:
*		FILE *stream: Ponteiro para o arquivo de dados
*		int id: chave da busca
*	Return:
*		long: byte offset do registro se encontrar o ID. EOF caso contrario.	
*/
long find(FILE *stream, int id) {
    int idReg;

    /*Busca pelo ID*/
    do {
        fread(&idReg, sizeof(int), 1, stream); //Le o primeiro ID
        fgetc(stream); // Pula o terminador de campo
        
        //Compara o id lido com a chave
        if (idReg == id)
            return ftell(stream) - sizeof(int) - sizeof(char);

        //Pula o restante dos campos
        ignoreStr(stream);
        ignoreStr(stream);
        ignoreStr(stream);
        ignoreInt(stream);
        ignoreInt(stream);
        ignoreStr(stream);
        fgetc(stream); //Pula terminador de arquivo
    } while (!feof(stream));

    return EOF;
}

/* dumpStr () - Imprime na tela um campo de tamanho variavel que armazena uma string.
 *	Parameter:
 *		FILE *stream: Ponteiro para o arquivo de dados
 *	Return:
 *		-
 *
*/
void dumpStr(FILE *stream) {
    char c;

    //Imprime um caracter na tela até que chega á um terminador de campo ou EOF
    do {
        c = fgetc(stream);
        if (c != TERMINADOR_CAMPO)
            fputc(c, stdout);
    } while (!feof(stream) && c != TERMINADOR_CAMPO);
}

/* dumpInt () - Imprime na tela um campo de tamanho fixo que armazena um inteiro.
 *	Parameter:
 *		FILE *stream: Ponteiro para o arquivo de dados
 *	Return:
 *		-
 *
*/
void dumpInt(FILE *stream) {
    int x;
    fread(&x, sizeof(int), 1, stream);
    fgetc(stream);
    printf("%d", x);
}

/* dumpAll () - Imprime na tela todos os registros do arquivo de dados na ordem em que estao armazenados
 *	Parameter:
 *		FILE *stream: Ponteiro para o arquivo de dados
 *	Return:
 *		-
 *
*/
void dumpAll(FILE *stream) {
    int i;
    for (i = 0; i < 100; i++)
        dumpData(stream); // Imprime um registro
}

/* printCampo () - Imprime na tela, de forma formatada, à que campo se refere e o conteudo do campo
 *	Parameter:
 *		char *nome: cabeçalho a ser impresso
 *		(*f) (FILE *): Função usada para imprimir o campo
 *		FILE *stream: Ponteiro para o arquivo de dados
 *	Return:
 *		-
 *
*/
void printCampo(char *nome, void (*f) (FILE *), FILE *stream) {
    printf("%s\t", nome);
    f(stream);
    printf("\n");
}

/* dumpData () - Imprime todos os campos de um registro
 *	Parameter:
 		FILE *stream: file stram
 	Return: 
 		-
*/
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

/* meuGetLine () - Função que chama o getline() e retira o /n armazenado na string, se existir
	Parameter:
		size_t *b: Armazenara o numero de bytes armazenados
		FILE *stream: file stream
	Return: Vetor com string lida
*/
char *meuGetLine(size_t *b, FILE *stream) {
    char *s = NULL;
    int i;
    getline(&s, b, stream); //Chama o getline
    i = strlen(s)-1;

    //Remove, se encontrar, o /n
    if (i >= 0 && s[i] == '\n')
        s[i] = '\0';

    return s;
}

/* nextRandID () - Gera um numero aleatorio entre 0 e 99 que ainda não foi armazenado em ids e o retorna para o usuario
 *	Parametros: 
 *		bool *ids: vetor usado para armazenar valores/posições já sorteadas
 *	Return:		
 *		int: Numero aleatorio entre 0 e 99
*/
int nextRandId(bool *ids) {
    int id = -1;

    /* Sorteia um numero entre 0 e 99 até que a posição sorteada esteja "vazia"
     * então preenche a posição e retorna o valor
     */
    do {
        id = rand() % 100;
        if (ids[id])
            id = -1;
        else
            ids[id] = 1;
    } while (id < 0);

    return id;
}

/*	generateBD (void) - A partir de arquivos .txt contendo informações sobre 250 series diferentes, gera um arquivo binario contendo 100 dessas
 *  em ordem aleatória atribuindo IDs tbm aleatorios.
 *		Parameter: 
 *			-
 *		Return:
 *			-
*/
void generateBD (void) {
    
    /*Variaveus*/
    size_t i, j; //Contaores gerais
    int id;
    int numb; 
    int size;

    int randVector[100]; //Vetor contendo numeros de 0 à 249 em ordem aleaotoria
    bool *randIds = calloc(100, sizeof(bool)); //Vetor contendo numeros de 0 à 99 em ordem aleaotoria
    void *regVector = NULL; //Buffer usado para armazenar o conteudo de um registro antes de escrevê-lo no arquivo

    char terminadorCampo = TERMINADOR_CAMPO; //Caracter usado como terminador de Campos
    char terminadorRegistro = TERMINADOR_REGISTRO; //Carracter usado como terminador de Registro

    /*Cria um vetor para armazenar as 250 series em RAM*/
    REGISTRO *registros = NULL; //Vetor dos registros salvos em RAM na forma de struct.
    registros = (REGISTRO *) malloc (250 * sizeof(REGISTRO)); //Alloca memoria


    /*ponteiros para os arquivos .txt*/
    FILE *titulo = NULL; 
    FILE *descricao = NULL;
    FILE *producao = NULL;
    FILE *ano = NULL;
    FILE *temporada = NULL;
    FILE *genero = NULL;

    /*Arquivo binario*/
    FILE *BancoDeDados = NULL; 


    /*Gera uma vetor de 100 posições com numeros de 0 à 249 em ordem aleatoria e não repetidos*/
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


    /*Abre arquivos .txt*/
    titulo = fopen ("nomes.txt", "r");
    descricao = fopen ("desc.txt", "r");
    producao = fopen ("paises.txt", "r");
    ano = fopen ("anos.txt", "r");
    temporada = fopen ("temporadas.txt", "r");
    genero = fopen ("generos.txt", "r");


    /*Salva o conteudo dos arquivos .txt em RAM para manipulação*/
    for (i = 0; i < 250; i++) {
        registros[i].titulo = NULL;
        registros[i].descricao = NULL;
        registros[i].producao = NULL;
        registros[i].genero = NULL;

        //Titudlo
        j = 0;
        registros[i].titulo = meuGetLine (&j, titulo);

        //Descricao
        j = 0;
        registros[i].descricao = meuGetLine (&j, descricao);

        //Producao (PAIS)
        j = 0;
        registros[i].producao = meuGetLine (&j, producao);

        //Ano
        fscanf  (ano, "%d", &registros[i].ano);
        
        //Temporada	
        fscanf  (temporada, "%d", &registros[i].temporada);

        //Genero
        j = 0;
        registros[i].genero = meuGetLine (&j, genero);
    }


    /*Gerando o arquivo binario*/

    BancoDeDados = fopen ("BancoDeDados", "w"); //Cria o arquivo

   	//Serão salvos/escritos apenas 100, dos 250 registros
    for (j = 0; j < 100; j++) {
        i = randVector[j]; //Seleciona o primeiro registro a ser salvo na ram
        id = nextRandId(randIds); //Gera o id deste registro

        /*Calcula o tamanho total do registro*/
        size = sizeof(char) * (strlen(registros[i].titulo) + strlen(registros[i].descricao) + strlen(registros[i].producao) + strlen (registros[i].genero));
        size += sizeof(int) * 3;
        size += sizeof(char) * 8;

        //Alloca o buffer
        regVector = calloc(size, 1);
        size = 0;//Reinicia variavel size

        /*Copia o conteudo da struct para o vetor regVector, adicionando os terminadores*/
        
        /*Campo ID*/
        memcpy (regVector + size, &id, sizeof(int));
        size += sizeof(int);
        //Terminador
        memcpy (regVector + size, &terminadorCampo, sizeof(char));
        size += sizeof(char);

        /*Campos TITULO*/
        memcpy(regVector + size, registros[i].titulo, strlen(registros[i].titulo) * sizeof(char));
        size += strlen(registros[i].titulo) * sizeof(char);
        //Terminador
        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        /*Campo DESCRICAO*/
        memcpy(regVector + size, registros[i].descricao, strlen(registros[i].descricao) * sizeof(char));
        size += strlen(registros[i].descricao) * sizeof(char);
        //Terminador
        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        /*Campo PRODUCAO*/
        memcpy(regVector + size, registros[i].producao, strlen(registros[i].producao) * sizeof(char));
        size += strlen(registros[i].producao) * sizeof(char);
        //Terminador
        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        /*Campos ANO*/
        memcpy (regVector + size, &registros[i].ano, 1 * sizeof(int));
        size += sizeof(int);
        //Terminador
        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        /*Campos TEMPORADA*/
        memcpy (regVector + size, &registros[i].temporada, 1 * sizeof(int));
        size += sizeof(int);
        //Terminador
        memcpy (regVector + size, &terminadorCampo, 1 * sizeof(char));
        size += sizeof(char);

        /*Campos GENERO*/
        memcpy(regVector + size, registros[i].genero, strlen(registros[i].genero) * sizeof(char));
        size += strlen(registros[i].genero) * sizeof(char);
        //Terminador
        memcpy (regVector + size, &terminadorCampo, sizeof(char));
        size += sizeof(char);
        
        /*Terminador de registro*/
        memcpy (regVector + size, &terminadorRegistro, sizeof(char));
        size += sizeof(char);

        //Escreve no arquivo e limpa o regVector para a reutilização
        fwrite (regVector, 1, size, BancoDeDados);
        free(regVector);
    }

    /*Fecha arquivos utilizados*/
    fclose (titulo);
    fclose (descricao);
    fclose (producao);
    fclose (ano);
    fclose (temporada);
    fclose (genero);
    fclose(BancoDeDados);

    /*Libera a memoria usado para armazenamento dos registros em RAM*/
    for (i = 0; i < 250; i++) {
        free(registros[i].titulo);
        free(registros[i].descricao);
        free(registros[i].genero);
        free(registros[i].producao);
    }
    free(registros);
    free(randIds);

}

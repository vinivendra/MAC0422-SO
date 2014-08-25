#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define STDIN 0
#define STDOUT 1

/**
 Muda todas as permissões do arquivo dado,
 deixando-o totalmente protegido ou desprotegido.
 
 @param argc A quantidade de argumentos passada.
 @param argv O vetor contendo os argumentos.
 @param permissao A permissao a ser dada para o programa
 na base octal; espera-se que seja 0000 ou 0777.
 
 @return Retorna 0 em caso de sucesso, -1 caso contrário.
 */

int mudapermissao(int argc, char **argv, int permissao) {
    if (argc < 1) {
        if (permissao == 0000)
            printf("Modo de uso: protegepracaramba <arquivo>\n");
        else if (permissao == 0777)
            printf("Modo de uso: liberageral <arquivo>\n");
    }

    if (chmod(argv[0], permissao) == 0) {
        return 0;
    }
    else {
        printf("Não foi possível mudar as permissões do arquivo %s", argv[1]);
        return -1;
    }

    return -1;
}





/**

 Rodeveja tentará executar o programa com nome localizado
 em argv[0], onde argv[1..argc-1] são os argumentos desse
 segundo programa.

 Se bg for 1, o segundo programa irá rodar em background.

 @param argc A quantidade de argumentos passada.
 @param argv O vetor contendo os argumentos.
 @param bg Uma flag indicando se o programa deve ou não rodar
 em background.


 @return Retorna 0 caso o programa tenha sido executado
 normalmente, -1 casoc ontrário.
 */



int rodeveja(int argc, char **argv, int bg)
{
    char* nome;
    int retVal = 42;
    if(argc < 1)
    {
        if(!bg) printf("Modo de uso: rodeveja <executavel> <argumentos>\n");
        else printf("Modo de uso: rode <executavel> <argumentos>\n");
        return -1;
    }

    if(fork())
    {
        if(!bg){
            wait(&retVal);
            retVal = retVal >> 8;
            printf("Valor retornado => %d\n", retVal);
        }
    }
    else
    {
        if(bg) close(STDIN);
        nome = argv[0];
        execve(nome, argv, NULL);
        printf("Não foi possível encontrar o programa: '%s'\n", nome);
    }
    
    return 0;
    
}




/**
 Cria uma string terminada em \0 com uma linha de entrada
 na STDIN terminada em EOF ou \n.

 @return Retorna a string criada.
*/


char* leLinha(void)
{
	int tam = 0, i = 0;
	int tamBuf = 10;
	char* linha = malloc(tamBuf);
	char* linhaTemp;
	char c;
	while((c = getchar())!= '\n' && c != EOF && c != '\0')
	{
		if(tam == tamBuf) /*Cabou o espaço na linha. Vamos dobrar */
		{
			tamBuf *= 2;
			linhaTemp = linha;
			linha = malloc(tamBuf);
			for(i = 0; i < tam; i++)
				linha[i] = linhaTemp[i];
			free(linhaTemp);
		}

		linha[tam++] = c;

	}
	linhaTemp = linha;
	linha = malloc(tam + 1);
	for(i = 0; i < tam; i++)
		linha[i] = linhaTemp[i];
	linha[tam] = '\0';
	free(linhaTemp);

	return linha;
}

/**
 Cria uma nova string, que vai ser
 uma cópia dos caracteres de str[ini..fim]
 
 @return Retorna a string criada.
*/

char* getSubStr(char* str, int ini, int fim)
{
	int tam = fim - ini;
	char* substr = malloc(tam+1);
	int i = 0;
	for(i = ini; i <= fim; i++) substr[i] = str[i];
	substr[i] = '\0';
	return substr;	
}

/**
 Recebe uma string terminada em \0 e com palavras separadas
 por espaços e retorna um vetor, em que cada posição
 contém uma palavra.

 Supõe-se que a string não seja NULL ou "\0", e seja terminada
 por '\0'.
 
 @param linha A string a ser separada
 @param Um ponteiro para um inteiro onde se guardará o
 número de palavras resultantes da separação
 
 @return Retorna o vetor resultante, contendo as palavras.


*/

char** constroiArgv(char* linha, int* argc2)
{
	char** argv, **argvTemp;
	char* token;
	const char s[2] = " ";

	int i, argc = 0;
	int tamBuf = 4, tam = 0;
	int sizeStr = strlen(linha);

	argv = malloc(sizeof(char*)*tamBuf);
	token = strtok(linha, s);
   
   
   while( token != NULL ) 
   {
   	if(tam-1 == tamBuf) /*Se está no tamanho limite, dobra*/
   	{
   		argvTemp = argv;
   		tamBuf *= 2;
   		argv = malloc(sizeof(char*)*tamBuf);
   		for(i = 0; i < tam; i++)
   			argv[i] = argvTemp[i];
   		free(argvTemp);
   	}
   	argv[tam++] = token;    
   	token = strtok(NULL, s);
   }
   	argvTemp = argv;
	argv = malloc(tam);
	for(i = 0; i < tam; i++)
		argv[i] = argvTemp[i];
	free(argvTemp);


	*argc2 = tam;



   return argv;
   
	

}


int main(int argc, char* argv[])
{
	char* linha, *cmd, *args;
	char** argv2;
	int i, argc2 = 0;
	while(1)
	{
		printf("$");
		linha = leLinha();
		for(i = 0; linha[i] != '\0' && linha[i] != ' '; i++);
		cmd = getSubStr(linha, 0, i-1);

		argv2 = constroiArgv(linha, &argc2);

		argc2--;
		argv2++;



		if(!strcmp(cmd, "rodeveja"))
		{
			/*printf("Se preparando para executar rodeveja...\n");*/
			rodeveja(argc2, argv2, 0);
		}
		else if(!strcmp(cmd, "rode")) 
		{
			rodeveja(argc2, argv2, 1);
		}
        else if(!strcmp(cmd, "protegepracaramba"))
        {
            mudapermissao(argc2, argv2, 0000);
        }
        else if(!strcmp(cmd, "liberageral"))
        {
            mudapermissao(argc2, argv2, 0777);
        }
		else printf("Comando |%s| não reconhecido\n", cmd);

		free(linha);
		free(cmd);

	}
	return 0;
}




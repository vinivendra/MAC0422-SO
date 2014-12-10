#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char **argv) {

    char *hue = "/usr/src/EP4-Testes/arquivoNovo.c";

    int result = 1337;

    result = lsr(hue);

    printf("Resultado do lsr: %d\n", result);

    result = open(hue, 01024, 0666);
    
    printf("OLHA SO %d\n", result);

    return 0;
}

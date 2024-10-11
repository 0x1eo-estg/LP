#include <stdio.h>
#include <string.h>


#define INVALID_VALUE "O valor inserido é inválido."

/**
 * Esta função apaga o terminal
*/
void clearConsole() {
    system("cls || clear");
}

/**
 * Esta função apaga qualquer tipo de caractere lido no stdinput
*/
void cleanInputBuffer() {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/**
 * Esta função lê um número pedido ao utilizador
 * @param minValue
 * @param maxValue
 * @param msg
 * @return o número fornecido pelo utilizador
*/
int getInt(int minValue, int maxValue, char* msg) {
    int value;
    printf(msg);
    while (scanf("%d", &value) != 1 || value < minValue || value > maxValue) {
        puts(INVALID_VALUE);
        cleanInputBuffer();
        printf(msg);
    }
    cleanInputBuffer();
    return value;
}

/**
 * Esta função lê um número(float) pedido ao utilizador
 * @param minValue
 * @param maxValue
 * @param msg
 * @return o número fornecido pelo utilizador
*/
float getFloat(float minValue, float maxValue, char* msg) {
    float value;
    printf(msg);
    while (scanf("%f", &value) != 1 || value < minValue || value > maxValue) {
        puts(INVALID_VALUE);
        cleanInputBuffer();
        printf(msg);
    }
    cleanInputBuffer();
    return value;
}

/**
 * Esta função lê um número(double) pedido ao utilizador
 * @param minValue
 * @param maxValue
 * @param msg
 * @return o número fornecido pelo utilizador
*/
double getDouble(double minValue, double maxValue, char* msg) {
    double value;
    printf(msg);
    while (scanf("%lf", &value) != 1 || value < minValue || value > maxValue) {
        puts(INVALID_VALUE);
        cleanInputBuffer();
        printf(msg);
    }
    cleanInputBuffer();
    return value;
}

/**
 * Esta função lê um caractere pedido ao utilizador
 * @param msg
 * @return o caractere fornecido pelo utilizador
*/
char getChar(char* msg) {
    char value;
    printf(msg);
    value = getchar();
    cleanInputBuffer();
    return value;
}

/**
 * Esta função lê uma string pedida ao utilizador
 * @param string
 * @param length
 * @param msg
 * @return a string fornecida pelo utilizador
*/
void readString(char* string, unsigned int length, char* msg) {
    printf(msg);
    if (fgets(string, length, stdin) != NULL) {
        unsigned int len = strlen(string) - 1;
        if (string[len] == '\n') {
            string[len] = '\0';
        } else {
            cleanInputBuffer();
        }
    }
}

/**
 * @file input.h
 * @brief Este header inclui funcionalidades úteis usadas em todo o projeto, desde limpar a consola ao armazenamento de variáveis através do input do utilizador
*/
#ifndef INPUT_H
#define INPUT_H

void clearConsole();
void cleanInputBuffer();
int getInt(int minValue, int maxValue, char* msg);
float getFloat(float minValue, float maxValue, char* msg);
double getDouble(double minValue, double maxValue, char* msg);
char getChar(char* msg);
void readString(char* string, unsigned int length, char* msg);



#endif /* INPUT_H */
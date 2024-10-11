
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "file.h"
#include "input.h"
#include "production.h"

#include "customers.h"

/**
 * Esta função imprime os materiais presentes na lista
 * 
 * @param production 
 * @param materials 
 */
void productionList(Production production, Materials* materials) {

    for (int i = 0; i < NMATS; i++) {
        printf("\n%s  %s --- %d  %s\n", materials->materials[i].id, materials->materials[i].description,
            production.materials[i], materials->materials[i].unit);
    }
}

int compareToCurrentDate(Date date) {
    time_t t = time(NULL);
    struct tm time = *localtime(&t);

    if (date.year < (time.tm_year + 1900)) {
        return -1;
    } else if (date.year > (time.tm_year + 1900)) {
        return 1;
    }

    if (date.year == (time.tm_year + 1900)) {
        if (date.month < (time.tm_mon + 1)) {
            return -1;
        } else if (date.month > (time.tm_mon + 1)) {
            return 1;
        } else if (date.day < time.tm_mday) {
            return -1;
        } else if (date.day > time.tm_mday) {
            return 1;
        } else {
            return 1;
        }
    }
}

/**
 * Esta função recebe 2 datas, uma de inicio e outra de fim, e compara-as
 * 
 * @param date 
 * @param start 
 * @param end 
 * @return -1 se a data inicial for depois da data final, contrariamente, 1
 */
int compareDates(Date date, Date start, Date end) {


    if (date.year < start.year || date.year > end.year) {
        return -1;
    } else if (date.year >= start.year && date.year <= end.year) {

        //se for no mesmo ano comparar meses de forma normal
        if (start.year == end.year) {
            if (date.month < start.month || date.month > end.month) {
                return -1;
            } else if (date.month >= start.month && date.month <= end.month) {

                //se for no mesmo mês fazer a coparação normal de dias
                if (start.month == end.month) {
                    if (date.day < start.day || date.day > end.day) {
                        return -1;
                    } else if (date.day >= start.day && date.day <= end.day) {
                        return 1;
                    }
                    //se não for o mesmo mes
                } else {
                    if (date.month == start.month) { //mesmo mês que a primeira data, so compara com dia da data inicial
                        if (date.day < start.day) {
                            return -1;
                        } else {
                            return 1;
                        }
                    } else if (date.month > start.month && date.month < end.month) {
                        return 1;
                    } else if (date.month == end.month) { //mesmo mês que a ultima data, so compara com ultimo dia
                        if (date.day > end.day) {
                            return -1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
            //se ano for diferente
        } else {
            //ano de inicio
            if (date.year == start.year) {
                if (date.month < start.month) {
                    return -1;
                } else {
                    if (date.month == start.month) {//mesmo mês que a primeira data, so compara com dia da data inicial

                        if (date.day < start.day) {
                            return -1;
                        } else {
                            return 1;
                        }
                    } else if (date.month > start.month) {
                        return 1;
                    }

                }
            } else if (date.year > start.year && date.year < end.year) {
                return 1;
            } else if (date.year == end.year) {
                if (date.month > end.month) {
                    return -1;
                } else {
                    if (date.month == end.month) {
                        if (date.day > end.day) {
                            return -1;
                        } else {
                            return 1;
                        }
                    } else if (date.month < end.month) {
                        return 1;
                    }
                }
            }
        }
    }
}

/**
 * Esta função calcula a quantidade de materiais necessários para satisfazer uma encomenda
 * 
 * @param materials 
 * @param production 
 * @param order 
 */
void calculateMaterials(Materials* materials, Production* production, Order order) {
    //rodar posições de encomenda e multiplicar essa posição pela quantidade de mateiais
    for (int j = 0; j < materials->nMaterials; j++) { //material
        for (int i = 0; i < 3; i++) { //posição do produto nas encomendas e quantidade encomendada
            production->materials[j] += order.products[i] * materials->materials[j].quantity[i];
        }
    }
}

/**
 * Esta função calcula a quantidade de materiais necessário para satisfazer x encomendas
 * num intervalo de tempo
 * 
 * @param orders apontador para o struct de encomendas
 * @param materials apontador para o struct de materiais
 * @param production apontador para o struct de produção
 * @param start 
 * @param end 
 * @return 
 */
int checkOrdersDate(Orders* orders, Materials* materials, Production* production, Date start, Date end) {
    int i = 0, counter = 0;
    while (i < orders->nOrders) {
        int result = compareDates(orders->orders[i].date, start, end);
        //se a encomenda pertencer ao intervalo de data
        if (result == 1) {
            calculateMaterials(materials, production, orders->orders[i]);
            counter++;
        }
        i++;
    }
    return counter;
}

/**
 * Esta função verifica se uma data é válida ou não
 * 
 * @param day 
 * @param mon 
 * @param year 
 * @return se a data é válida ou não 0;
 */
int validDate(int day, int mon, int year) {

    int is_valid = 1, is_leap = 0;

    if (year >= 1800 && year <= 9999) {

        //  check whether year is a leap year
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            is_leap = 1;
        }

        // check whether mon is between 1 and 12
        if (mon >= 1 && mon <= 12) {
            // check for days in feb
            if (mon == 2) {
                if (is_leap && day == 29) {
                    is_valid = 1;
                } else if (day > 28) {
                    is_valid = 0;
                }
            }// check for days in April, June, September and November
            else if (mon == 4 || mon == 6 || mon == 9 || mon == 11) {
                if (day > 30) {
                    is_valid = 0;
                }
            }// check for days in rest of the months 
                // i.e Jan, Mar, May, July, Aug, Oct, Dec
            else if (day > 31) {
                is_valid = 0;
            }
        } else {
            is_valid = 0;
        }

    } else {
        is_valid = 0;
    }

    return is_valid;

}

/**
 * Esta função incicializa todas as posições do array materials a 0
 * 
 * @param production apontador para o struct de produção
 */
void initProduction(Production* production) {
    for (int i = 0; i < NMATS; i++) {
        production->materials[i] = 0;
    }
}

/**
 * Esta função apresenta os materiais necessários para satisfazer as encomendas num intervalo de tempo, 
 * perguntando ao utilizador se pretende exportar a lista de produção para um ficheiro
 * 
 * @param orders apontador para o struct de encomendas
 * @param materials apontador para o struct de materiais
 */
void generateProduction(Orders* orders, Materials* materials) {
    if (orders->nOrders > 0) {

        Production production;
        Date start;
        Date end;

        initProduction(&production);

        do {
            printf("Data de inicio (DD/MM/YYYY): ");
            scanf("%d/%d/%d", &start.day, &start.month, &start.year);
        } while (!validDate(start.day, start.month, start.year));

        do {
            printf("Data de fim (DD/MM/YYYY): ");
            scanf("%d/%d/%d", &end.day, &end.month, &end.year);

        } while (!validDate(end.day, end.month, end.year));

        int counter = checkOrdersDate(orders, materials, &production, start, end);

        clearConsole();
        printf("Lista de Produção (%02d/%02d/%d - %02d/%02d/%d)\n", start.day,
            start.month, start.year, end.day, end.month, end.year);
        printf("ID do Material - Descrição do Material - Quantidade - Unidade de medida\n");
        puts("----------------------------------------------------------------");
        productionList(production, materials);
        puts("----------------------------------------------------------------");
        int userConfirmInput = getInt(1, 2, "Deseja exportar a lista de produção para um ficheiro?\n 1 - Sim\n 2 - Não, sair\n");
        if (userConfirmInput == 1) {
            char filename[FILE_LENGHT];

            readString(filename, FILE_LENGHT, MSG_GET_FILENAME);
            strcat(filename, ".txt\0");

            saveProduction(&production, materials, start, end, filename);
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_ORDER_LIST_EMPTY);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para sair desta lista!");
        scanf("%c", &c);
    }
}

//-----------------------------------------

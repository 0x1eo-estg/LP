#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "input.h"
#include "production.h"
#include "listings.h"
#include "file.h"
#include "customers.h"
#include "products.h"
#include "materials.h"
#include "orders.h"

/**
 * Esta função imprime o cliente que tem mais encomendas registadas
 * 
 * @param orders struct de encomendas
 * @param customers apontador para o struct de clientes
 */
void mostOrdersCustomer(Orders orders, Customers* customers) {

    int counter[customers->nCustomers];

    for (int i = 0; i < customers->nCustomers; i++) {
        counter[i] = 0;
    }

    for (int i = 0; i < orders.nOrders; i++) {
        int pos = checkCustomer(*customers, orders.orders[i].customerId);
        if (pos != -1) {
            counter[pos]++;
        }
    }

    int most = counter[0];
    int j = 0;
    for (int i = 0; i < customers->nCustomers; i++) {
        if (counter[i] > most) {
            most = counter[i];
            j = i;
        }
    }

    char c = '0';
    printf(" ----------------------------------------------------------------------- \n");
    printf("Cliente com mais encomendas é:\n%-4d %-10s %-5d %-6s - %d encomenda(s)\n", customers->customers[j].id, customers->customers[j].name,
            customers->customers[j].nif, customers->customers[j].address.city, most);
    puts(" ----------------------------------------------------------------------- ");
    printf("Pressione a tecla «ENTER» para voltar!");
    scanf("%c", &c);
}


//-------listagem de cidades

/**
 * Esta função imprime quantas encomendas certa cidade tem
 * 
 * @param cities struct de cidades
 */
void printCities(Cities cities) {
    for (int i = 0; i < cities.nCities; i++) {
        printf("\n%s ----- %d\n", cities.cities[i].city, cities.cities[i].counter);
    }
}

/**
 * Esta função imprime o número de encomendas por cidade num determinado intervalo
 * de tempo
 * 
 * @param orders apontador para o struct de encomendas
 * @param customers apontador para o struct de clientes
 */
void ordersPerCityDate(Orders* orders, Customers* customers) {
    int i = 0;
    Date start;
    Date end;

    Cities cities;
    cities.cities = (City*) malloc(10 * sizeof (City));
    cities.nCities = 0;

    do {
        printf("Data de inicio (DD/MM/YYYY): ");
        scanf("%d/%d/%d", &start.day, &start.month, &start.year);
    } while (!validDate(start.day, start.month, start.year));

    do {
        printf("Data de fim (DD/MM/YYYY): ");
        scanf("%d/%d/%d", &end.day, &end.month, &end.year);

    } while (!validDate(end.day, end.month, end.year));


    while (i < orders->nOrders) {
        int result = compareDates(orders->orders[i].date, start, end);
        //se a encomenda pertencer ao intervalo de data
        if (result == 1) {
            int pos = checkCustomer(*customers, orders->orders[i].customerId);
            if (pos != -1) {
                int j = 0, found = 0;
                if (cities.nCities > 0) {
                    for (j = 0; j < cities.nCities; j++) {
                        if (strcmp(cities.cities[j].city, customers->customers[pos].address.city) == 0) {
                            cities.cities[j].counter++;
                            found++;
                        }
                    }
                }
                if (!found) {
                    strcpy(cities.cities[cities.nCities].city, "");
                    cities.cities[cities.nCities].counter = 1;
                    strcpy(cities.cities[cities.nCities].city, customers->customers[pos].address.city);
                    cities.nCities++;
                }
            }
        }
        i++;
    }
    clearConsole();
    printf("Listagem de cidades para entrega de encomendas (%02d/%02d/%d - %02d/%02d/%d)\n", start.day,
            start.month, start.year, end.day, end.month, end.year);
    printf("Cidade ---- Quantidade de encomendas\n");
    printf("----------------------------------------------------------------");
    printCities(cities);
    puts("----------------------------------------------------------------");
    int userConfirmInput = getInt(1, 2, "Deseja exportar esta lista?\n 1 - Sim\n 2 - Não, sair\n");
    if (userConfirmInput == 1) {
        char filename[FILE_LENGHT];

        readString(filename, FILE_LENGHT, MSG_GET_FILENAME);
        strcat(filename, ".txt\0");

        saveCitiesOrders(cities, start, end, filename);
    }
    free(cities.cities);

}
//--------------------fim listagens cidades

/**
 * Esta função imprime as cidades em que existem encomendas registadas e
 * a sua quantidade, perguntando ao utilizador se este pretende exportar esta
 * lista para um ficheiro
 * 
 * @param orders apontador para o struct de encomendas 
 * @param customers apontador para o struct de clientes
 */
void cityOrders(Orders* orders, Customers* customers) {
    int i = 0;

    Cities cities;
    cities.cities = (City*) malloc(15 * sizeof (City));
    cities.nCities = 0;

    while (i < orders->nOrders) {
        int pos = checkCustomer(*customers, orders->orders[i].customerId);
        if (pos != -1) {
            int j = 0, found = 0;
            if (cities.nCities > 0) {
                for (j = 0; j < cities.nCities; j++) {
                    if (strcmp(cities.cities[j].city, customers->customers[pos].address.city) == 0) {
                        cities.cities[j].counter++;
                        found++;
                    }
                }
            }
            if (!found) {
                strcpy(cities.cities[cities.nCities].city, "");
                cities.cities[cities.nCities].counter = 1;
                strcpy(cities.cities[cities.nCities].city, customers->customers[pos].address.city);
                cities.nCities++;
            }
        }
        i++;
    }

    clearConsole();
    printf("Listagem de cidades com encomendas registradas\n");
    printf("Cidade ---- Quantidade de encomendas\n");
    printf("----------------------------------------------------------------");
    printCities(cities);
    puts("----------------------------------------------------------------");
    int userConfirmInput = getInt(1, 2, "Deseja exportar esta lista?\n 1 - Sim\n 2 - Não, sair\n");
    if (userConfirmInput == 1) {
        char filename[FILE_LENGHT];

        readString(filename, FILE_LENGHT, MSG_GET_FILENAME);
        strcat(filename, ".txt\0");

        saveListOfCities(cities, filename);
    }
    free(cities.cities);
}
//---------------------------------------

/**
 * Esta função imprime o produtos que foi mais encomendado, apresentando os
 * restantes se existir um "empate"
 * 
 * @param orders struct de encomendas
 * @param products struct de produtos
 */
void mostOrdersProduct(Orders orders, Products products) {

    if (products.nProds > 0) {
        int counter[products.nProds];
        int list[products.nProds];
        for (int i = 0; i < products.nProds; i++) {
            counter[i] = 0;
            list[i] = 0;
        }

        for (int j = 0; j < orders.nOrders; j++) {
            for (int i = 0; i < products.nProds; i++) {
                counter[i] += orders.orders[j].products[i];
            }
        }


        int most = counter[0];
        int mid = counter[0];
        int less = counter[0];

        for (int i = 0; i < products.nProds; i++) {
            if (counter[i] > most) {
                most = counter[i];
                //passar para a segunda posição o anterior maior
                list[0] = i;
            } else if (products.nProds > 1 && counter[i] < less) {
                less = counter[i];
                list[products.nProds - 1] = i;
            }  
        }
        if (products.nProds == 3) {
            for (int j = 0; j < 3; j++) {
                if (j != list[0] && j != list[2]) {
                    mid = counter[j];
                    list[1] = j;
                }
            }
        }

        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        printf("Produtos ordenados por quantidade de encomendas: \n");
        for (int i = 0; i < products.nProds; i++) {
            printf("%s   %s   ----- %d unidade(s)\n", products.products[list[i]].id, products.products[list[i]].name, counter[list[i]]);
        }
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    } else {

        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_LIST_PRODUCT_EMPTY);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}
// ---------------- 

/**
 * Esta função imprime o valor total faturado, bem como o valor faturado pela
 * compra de produtos específicos
 * 
 * @param orders struct de encomendas
 * @param products struct de produtos
 */
void moneyEarned(Orders orders, Products products) {

    if (products.nProds > 0) {

        Date start;
        Date end;


        do {
            printf("Data de inicio (DD/MM/YYYY): ");
            scanf("%d/%d/%d", &start.day, &start.month, &start.year);
            cleanInputBuffer();
        } while (!validDate(start.day, start.month, start.year));

        do {
            printf("Data de fim (DD/MM/YYYY): ");
            scanf("%d/%d/%d", &end.day, &end.month, &end.year);
            cleanInputBuffer();
        } while (!validDate(end.day, end.month, end.year));

        clearConsole();
        int total[products.nProds];
        int list[products.nProds]; //organize products by price total

        for (int i = 0; i < products.nProds; i++) {
            total[i] = 0;
            list[i] = 0;
        }

        for (int j = 0; j < orders.nOrders; j++) {

            int result = compareDates(orders.orders[j].date, start, end);
            //se a encomenda pertencer ao intervalo de data
            if (result == 1) {
                for (int i = 0; i < products.nProds; i++) {
                    total[i] += orders.orders[j].products[i] * products.products[i].price;
                }
            }
        }

        int money = 0;
        for (int i = 0; i < products.nProds; i++) {
            money += total[i];
        }

        int most = total[0];
        int mid = total[0];
        int less = total[0];

        for (int i = 0; i < products.nProds; i++) {
            if (total[i] > most) {
                most = total[i];
                //passar para a segunda posição o anterior maior
                list[0] = i;
            } else if (products.nProds > 1 && total[i] < less) {
                less = total[i];
                list[products.nProds - 1] = i;
            }
        }
        if (products.nProds == 3) {
            for (int j = 0; j < 3; j++) {
                if (j != list[0] && j != list[2]) {
                    mid = total[j];
                    list[1] = j;
                }
            }
        }

        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        printf("                -------------- Faturamento --------------                \n");
        printf("                       (%02d/%02d/%d - %02d/%02d/%d)                     \n", start.day,
                start.month, start.year, end.day, end.month, end.year);
        printf(" Total: %d€ \n\n", money);
        for (int i = 0; i < products.nProds; i++) {
            printf("  %s   %s   ----- %d€\n", products.products[list[i]].id, products.products[list[i]].name, total[list[i]]);
        }
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    } else {

        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_LIST_PRODUCT_EMPTY);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}

/**
 * Esta função imprime as encomendas ainda não entregues
 * 
 * @param orders struct de encomendas
 * @param products apontador para o struct de produtos
 */
void notYetDelivered(Orders orders, Products * products) {

    time_t now = time(NULL);
    struct tm *today = localtime(&now);

    char c = '0';
    int flag = 0;
    for (int i = 0; i < orders.nOrders; i++) {
        Order currentOrder = orders.orders[i];
        if ((currentOrder.date.year > today->tm_year) ||
                (currentOrder.date.year == today->tm_year && currentOrder.date.month > today->tm_mon) ||
                (currentOrder.date.year == today->tm_year && currentOrder.date.month == today->tm_mon && currentOrder.date.day > today->tm_mday)) {
            printf(" ----------------------------------------------------------------------- \n");
            printf("ID da encomenda: %s\n", currentOrder.id);
            printf("Data de entrega: %d/%d/%d\n", currentOrder.date.day, currentOrder.date.month, currentOrder.date.year);
            printf("Total da encomenda: %d€\n", currentOrder.total);
            printf("Produtos da encomenda: ");
            for (int j = 0; j < products->nProds; j++) {
                if (orders.orders[i].products[j] > 0) {
                    if (flag) {
                        printf(", ");
                    }
                    printf("%dx %s", orders.orders[i].products[j], products->products[j].name);
                    flag = 1;
                }
            }
            flag = 0;
        }
        printf("\n");
    }
    puts(" -----------------------------------------------------------------------");
    printf("Pressione a tecla «ENTER» para voltar!");
    scanf("%c", &c);
}
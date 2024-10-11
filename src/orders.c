#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "orders.h"
#include "input.h"
#include "file.h"

/**
 * Esta função liberta a memória alocada para a lista de encomendas
 * @param orders apontador para a estrutura de encomendas
 */
void freeOrders(Orders* orders) {
    free(orders->orders);
}

/**
 * Esta função da output dos produtos disponiveis para encomendar
 * @param products apontador para a estutura de produtos
 * @return a quantidade de produtos disponíveis
 */
int printActiveProds(Products* products) {

    int i = 0, counter = 0;
    puts("Opções de produtos para encomendar: ");
    for (i = 0; i < products->nProds; i++) {
        if (products->products[i].isActive == true) {
            printf("\n%d - %s, %s, %s, %d€\n", ++counter, products->products[i].id, products->products[i].name,
                products->products[i].dimensions, products->products[i].price);
        }
    }

    if (counter == 0) {
        return 0;
    }
    return counter;

}


/**
 * Esta função gera a data de entrega da encomenda adicionando 3 dias à data de registo da encomenda
 * @param orders apontador para a estrutura de encomendas
 */
void getDeliveryDate(Orders* orders) {

    //melhorar para verificar se data é <= a data atual
    time_t t = time(NULL);
    struct tm time = *localtime(&t);

    int dd = time.tm_mday + 3, mm = time.tm_mon + 1, yy = time.tm_year + 1900, weekdd = time.tm_wday + 3;

    //check day of the week
    //se for fim de semana, não contar como dia de entrega
    if (weekdd > 6) {
        weekdd -= 7;
    }
    if (weekdd == 0) {
        dd += 1;
        weekdd++;
    } else if (weekdd == 6) {
        dd += 2;
        weekdd += 2;
    }
    //check if day is after that month day limit
    if (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12) {
        if (dd > 31) {
            dd -= 31;
            mm++;
            //check if mounth is after december
            if (mm > 12) {
                mm -= 12;
                yy++;
            }
        }
    } else if (mm == 4 || mm == 6 || mm == 9 || mm == 11) {
        if (dd > 30) {
            dd -= 30;
            mm++;
        }
    } else if (mm == 2) {
        if (dd > 28) {
            dd -= 28;
            mm++;
        }
    } else if (mm == 2 && (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0))) {
        if (dd > 29) {
            dd -= 29;
            mm++;
        }
    }
    //atributting the verified values
    orders->orders[orders->nOrders].date.day = dd;
    orders->orders[orders->nOrders].date.month = mm;
    orders->orders[orders->nOrders].date.year = yy;
    //print 
    printf("A data de entrega para esta encomenda é: %02d/%02d/%d \n", orders->orders[orders->nOrders].date.day,
        orders->orders[orders->nOrders].date.month, orders->orders[orders->nOrders].date.year);
}


/**
 * Esta função calcula o total da encomenda e apresenta o valor a pagar ao cliente
 * @param products apontador para a estrutura products utilizado para pegar o preço dos produtos
 * @param orders apontador para a estrutura encomendas
 */
void calculateTotal(Products* products, Orders* orders) {

    int i = 0;
    for (i = 0; i < products->nProds; i++) {
        orders->orders[orders->nOrders].total += orders->orders[orders->nOrders].products[i] * products->products[i].price;
    }
    if (orders->orders[orders->nOrders].total != 0) {
        puts(" ------------------------------------------------ ");
        printf("Total a pagar: %d €\n", orders->orders[orders->nOrders].total);
        puts(" ------------------------------------------------ ");
    }
}



/*
void printTotal(Products* products, Orders* orders) {

    int i = 0;
    for (i = 0; i < products->nProds; i++) {
        orders->orders[orders->nOrders].total += orders->orders[orders->nOrders].products[i] * products->products[i].price;
    }
}
*/

/**
 * Função que realloca a memória disponivel para a estrutura de encomendas
 * @param orders apontador para a estrutura de encomendas
 */
void reallocOrders(Orders* orders) {
    Order* tempPtr = (Order*)realloc(orders->orders, sizeof(Order) * (orders->nMax + 1));
    // if the temporary pointer is not null,
    //copy it to the original pointer and expand its capacity
    if (tempPtr != NULL) {
        orders->nMax += 1;
        orders->orders = tempPtr;
    }
}

/**
 * Função que apresenta as informações sobre uma encomenda especifica
 * @param order estrutura de uma encomenda
 * @param products apontador para a estrutura de produtos usada para mostrar o nome do produto encomendado
 */
void printOrders(Order order, Products* products) {

    //melhorar para mostrar nome do produto - ja ta

    printf("\n%s ", order.id);

    for (int i = 0; i < products->nProds; i++) {
        if (order.products[i] > 0) {
            printf(" %dx %s ", order.products[i], products->products[i].name);
        }
    }
    printf("%d€  %02d/%02d/%d   %d\n", order.total,
        order.date.day, order.date.month, order.date.year, order.customerId);

}

/**
 * Função que inicializa a estrutura de encomendas fazendo a leitura dos dados no ficheiro de encomendas
 * @param orders apontador para a estrutura de encomendas que vai receber os dados
 * @param filename nome do ficheiro de encomendas
 * @return sucesso ou não do processo
 */
int initialiseOrders(Orders* orders, char* filename) {
    int confirm = 0;
    FILE* fp = fopen(filename, "rb");
    if (fp != NULL) {
        fread(&orders->nOrders, sizeof(int), 1, fp);
        if (orders->nOrders > 0) {
            orders->orders = (Order*)malloc(orders->nOrders * sizeof(Order));
            //read info from file
            fread(orders->orders, sizeof(Order), orders->nOrders, fp);
            orders->nMax = orders->nOrders;
            confirm = 1;
        }
        fclose(fp);
    }
    if (!confirm) {
        fp = fopen(filename, "wb");
        if (fp != NULL) {
            orders->orders = (Order*)malloc(MAX_NUM_ORDER * sizeof(Order));
            orders->nOrders = 0;
            orders->nMax = INITIAL_ORDERS;

            confirm = 1;
        }
    }
    return confirm;
}

/**
 * Função que limpa os dados dos campos de uma encomenda especifica
 * @param order apontador para a estrutura de uma encomenda
 */
void deleteOrderData(Order* order) {
    int i = 0;
    strcpy(order->id, "");

    order->date.day = 0;
    order->date.month = 0;
    order->date.year = 0;
    order->customerId = -1;

    for (i = 0; i < NPRODS; i++) {
        order->products[i] = 0;
    }
    order->total = 0;

}

/**
 * Função que faz a verificação da existência e da posição de uma encomenda na lista
 * @param orders apontador para a estrutura de encomendas(lista)
 * @param id Id da encomenda a ser procurado«a na lista
 * @return a posição da encomenda caso existir ou -1 caso não existir
 */
int checkOrder(Orders orders, char id[MAX_ID_ORDER]) {

    int i = 0;

    while (i < orders.nOrders) {
        if (strcmp(orders.orders[i].id, id) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

/**
 * Função utilizada para fazer a edição de uma encomenda já existente
 * @param orders apontador para a estrutura de encomendas
 * @param products apontador para a estrutura de produtos
 * @param filename nome do ficheiro contendo a lista de encomendas
 */
void editOrder(Orders* orders, Products* products, char* filename) {
    int i = 0;
    //need to insert only the number, the program does the rest
    char id[MAX_ID_ORDER];
    char num[MAX_ID_ORDER - 1];
    readString(num, MAX_ID_ORDER - 1, MSG_GET_ID_ORDER);
    strcpy(id, "E");
    for (int missingZerosCount = (MAX_ID_ORDER - 1) - strlen(num); missingZerosCount > 0; missingZerosCount--) {
        strcat(id, "0");
    }
    strcat(id, num);
    int pos = checkOrder(*orders, id);

    if (pos != -1) {

        int activeProds = printActiveProds(products);
        if (activeProds > 0) {
            //atributting values

            //init positions on 0
            for (int i = 0; i < NPRODS; i++) {
                orders->orders[pos].products[i] = 0;
            }
            //total = 0
            orders->orders[pos].total = 0;

            int option = -1;
            puts("0 - Finalizar Encomenda");
            //ordering
            do {
                option = getInt(0, activeProds, "Opção: ");
                int i = 0;
                switch (option) {
                case 0:

                    break;
                case 1:
                    //encontrar qual foi o primeiro produto a ser imprimido
                    i = 0;
                    int first = -1;
                    while (first == -1 && i < products->nProds) {
                        if (products->products[i].isActive == true) {
                            first = i;
                        } else {
                            i++;
                        }
                    }
                    orders->orders[pos].products[first] += getInt(MIN_PROD_ORDER, MAX_PROD_ORDER, MSG_GET_QUANTITY_ORDER);
                    //calculo do preço    

                    break;
                case 2:
                    i = 0;
                    int second = -1, counter = 0;
                    while (second == -1 && i < products->nProds) {
                        if (products->products[i].isActive == true) {
                            counter++;
                            if (counter == 2) {
                                second = i;
                            }
                        }
                        i++;
                    }
                    orders->orders[pos].products[second] += getInt(MIN_PROD_ORDER, MAX_PROD_ORDER, MSG_GET_QUANTITY_ORDER);

                    break;
                case 3:
                    i = 0;
                    int third = -1;
                    counter = 0;
                    while (third == -1 && i < products->nProds) {
                        if (products->products[i].isActive == true) {
                            counter++;
                            if (counter == 3) {
                                third = i;
                            }
                        }
                        i++;
                    }
                    orders->orders[pos].products[third] += getInt(MIN_PROD_ORDER, MAX_PROD_ORDER, MSG_GET_QUANTITY_ORDER);

                    break;
                }
            } while (option != 0);
            calculateTotal(products, orders);
            if (orders->orders[orders->nOrders].total != 0) {
                getDeliveryDate(orders);
            }

            int userConfirmInput = getInt(1, 2, "\nDeseja confirmar a encomenda?\n"
                "1 -Sim\n"
                "2 - Não\n");
            if (userConfirmInput == 1) {
                updateOrder(*orders, pos, filename);
            }
        } else {
            char c = '0';
            printf(" ----------------------------------------------------------------------- \n");
            puts(ERROR_PRODS_INACTIVE);
            puts(" ----------------------------------------------------------------------- ");
            printf("Pressione a tecla «ENTER» para voltar!");
            scanf("%c", &c);
        }

    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_ORDER_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}

/**
 * Função usada para procurar por uma encomenda usando o Id
 * @param orders estrutura de encomendas (lista)
 * @param products apontador para a estrutura de produtos
 */
void searchOrder(Orders orders, Products* products) {

    //need to insert only the number, the program does the rest
    char id[MAX_ID_ORDER];
    char num[MAX_ID_ORDER - 1];
    readString(num, MAX_ID_ORDER - 1, MSG_GET_ID_ORDER);
    strcpy(id, "E");
    for (int missingZerosCount = (MAX_ID_ORDER - 1) - strlen(num); missingZerosCount > 0; missingZerosCount--) {
        strcat(id, "0");
    }
    strcat(id, num);
    
    int pos = checkOrder(orders, id);


    if (pos != -1) {
        printOrders(orders.orders[pos], products);
        printf("\n");
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_ORDER_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}

/**
 * Esta função faz o realloc da estrutura de encomendas após eliminar uma encomenda
 * @param orders
 */
void reallocOrdersAfterDelete(Orders* orders) {
    Order* tempPtr = (Order*)realloc(orders->orders, sizeof(Order) * (orders->nMax - 1));
    // if the temporary pointer is not null,
    // copy it to the original pointer and expand its capacity
    if (tempPtr != NULL) {
        orders->nMax -= 1;
        orders->orders = tempPtr;
    }
}

/**
 * Função responsavel pela eliminação de uma encomenda escolhida
 * @param orders apontador para a estrutura de encomendas
 * @param filename nome do ficheiro contendo a lista de encomendas
 */
void deleteOrder(Orders* orders, char* filename) {
    int i = 0;

    //need to insert only the number, the program does the rest
    char id[MAX_ID_ORDER];
    char num[MAX_ID_ORDER - 1];
    readString(num, MAX_ID_ORDER - 1, MSG_GET_ID_ORDER);
    strcpy(id, "E");
    for (int missingZerosCount = (MAX_ID_ORDER - 1) - strlen(num); missingZerosCount > 0; missingZerosCount--) {
        strcat(id, "0");
    }
    strcat(id, num);

    int pos = checkOrder(*orders, id);
    if (pos != -1) {
        int userConfirmInput = getInt(1, 2, "\nDeseja guardar as alterações na Base de Dados?\n"
            "1 -Sim\n"
            "2 - Não\n");
        if (userConfirmInput == 1) {
            for (i = pos; i < orders->nOrders - 1; i++) {
                //IDs continue to be without gaps
                strcpy(orders->orders[i + 1].id, "");
                strcpy(orders->orders[i + 1].id, orders->orders[i].id);

                orders->orders[i] = orders->orders[i + 1];
            }
            deleteOrderData(&orders->orders[i]);
            orders->nOrders--;
            removeOrder(*orders, filename);
            reallocOrdersAfterDelete(orders);
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_ORDER_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);

    }
}

/**
 * Função usada para apresentar ao cliente, as suas encomendas registadas
 * @param orders apontador para a estrutura de encomendas
 * @param products apontador para a estrutura de produtos
 * @param customerId Id do cliente associado as encomendas
 */
void viewMyOrders(Orders* orders, Products* products, int customerId) {
    if (orders->nOrders > 0) {
        int i;
//print do que dizem respeito os valores
        printf("ID da encomenda - Produtos  - Preço - Data de entrega - ID cliente\n");
        for (i = 0; i < orders->nOrders; i++) {
            if (orders->orders[i].customerId == customerId) {
                printOrders(orders->orders[i], products);
            }
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_ORDER_LIST_EMPTY);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}

/**
 * Função que faz a listagem das encomendas registadas
 * @param orders estrutura de encomendas(lista)
 * @param products apontador para a estrutura de produtos
 */
void listOrders(Orders orders, Products* products) {
    if (orders.nOrders > 0) {
        int i;
        printf("ID da encomenda - Produtos  - Preço - Data de entrega - ID cliente\n");
        for (i = 0; i < orders.nOrders; i++) {
            printOrders(orders.orders[i], products);
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_ORDER_LIST_EMPTY);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}


/**
 * Função usada para o registo de uma nova encomenda
 * @param orders apontador para a estrutura de encomendas(lista)
 * @param products apontador para a estrutura de produtos
 * @param customerId Id do cliente a associar com a encomenda
 * @return sucesso na criação
 */
int createOrder(Orders* orders, Products* products, int customerId) {
    if (products->nProds > 0) {
        int activeProds = printActiveProds(products);
        if (activeProds > 0) {
            //atributting values
            //id
            char id[MAX_ID_ORDER - 1];
            sprintf(id, "%d", orders->nOrders + 1);
            strcpy(orders->orders[orders->nOrders].id, "E");
            for (int missingZerosCount = 6 - strlen(id); missingZerosCount > 0; missingZerosCount--) {
                strcat(orders->orders[orders->nOrders].id, "0");
            }
            strcat(orders->orders[orders->nOrders].id, id);

            //init positions on 0
            for (int i = 0; i < NPRODS; i++) {
                orders->orders[orders->nOrders].products[i] = 0;
            }
            //total = 0
            orders->orders[orders->nOrders].total = 0;
            //customer ordering
            orders->orders[orders->nOrders].customerId = customerId;

            int option = -1;
            int counter = 0;
            printf("\n0 - Finalizar Encomenda\n");
            //ordering
            do {
                option = getInt(0, activeProds, "Opção: ");
                int i = 0;
                switch (option) {
                case 0:

                    break;
                case 1:
                    //encontrar qual foi o primeiro produto a ser imprimido
                    i = 0;
                    int first = -1;
                    while (first == -1 && i < products->nProds) {
                        if (products->products[i].isActive == true) {
                            first = i;
                        } else {
                            i++;
                        }
                    }
                    orders->orders[orders->nOrders].products[first] += getInt(MIN_PROD_ORDER, MAX_PROD_ORDER, MSG_GET_QUANTITY_ORDER);
                    //calculo do preço    

                    break;
                case 2:
                    i = 0;
                    int second = -1;
                    counter = 0;
                    while (second == -1 && i < products->nProds) {
                        if (products->products[i].isActive == true) {
                            counter++;
                            if (counter == 2) {
                                second = i;
                            }
                        }
                        i++;
                    }
                    orders->orders[orders->nOrders].products[second] += getInt(MIN_PROD_ORDER, MAX_PROD_ORDER, MSG_GET_QUANTITY_ORDER);

                    break;
                case 3:
                    i = 0;
                    int third = -1;
                    counter = 0;
                    while (third == -1 && i < products->nProds) {
                        if (products->products[i].isActive == true) {
                            counter++;
                            if (counter == 3) {
                                third = i;
                            }
                        }
                        i++;
                    }
                    orders->orders[orders->nOrders].products[third] += getInt(MIN_PROD_ORDER, MAX_PROD_ORDER, MSG_GET_QUANTITY_ORDER);

                    break;
                }
            } while (option != 0);
            //calculate total and print total
            calculateTotal(products, orders);
            if (orders->orders[orders->nOrders].total == 0) {
                return -1;
            } else {
                //delivery date
                getDeliveryDate(orders);
            }
            return orders->nOrders++;
        } else {
            char c = '0';
            printf(" ----------------------------------------------------------------------- \n");
            puts(ERROR_PRODS_INACTIVE);
            puts(" ----------------------------------------------------------------------- ");
            printf("Pressione a tecla «ENTER» para voltar!");
            scanf("%c", &c);
            return -1;
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_LIST_PRODUCT_EMPTY);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
        return -1;
    }

}

/**
 * Função usada para o registo de uma nova encomenda e sua confirmação
 * @param orders apontador para a estrutura de encomendas(lista)
 * @param products apontador para a estrutura de produtos
 * @param customerId Id do cliente a associar com a encomenda
 * @param filename nome do ficheiro contendo a lista de encomendas
 */
void createOrders(Orders* orders, Products* products, int customerId, char* filename) {
    if (orders->nOrders == orders->nMax) {
        reallocOrders(orders);
    }

    int pos = createOrder(orders, products, customerId);
    if (pos != -1) {
        printf(" ----------------------------------------------------------------------- ");
        printOrders(orders->orders[pos], products);
        puts(" ----------------------------------------------------------------------- ");

        int userConfirmInput = getInt(1, 2, "\nDeseja confirmar a encomenda?\n"
            "1 -Sim\n"
            "2 - Não\n");
        if (userConfirmInput == 1) {
            insertOrder(orders, filename);
        } else {
            //delete data
            deleteOrderData(&orders->orders[pos]);
            if (orders->nOrders > 0) {
                orders->nOrders--;
            }
        }
    }
}
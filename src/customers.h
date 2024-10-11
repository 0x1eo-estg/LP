/**
 * @file customers.h
 * @brief Este header inclui as funcionalidades relativas à gestão de clientes
*/
#include <stdbool.h>
#ifndef CUSTOMERS_H
#define CUSTOMERS_H

#define INITIAL_CUSTOMERS 3
#define FILE_CUSTOMERS  "customers.bin"

#define ADMIN_PASSWORD "admin"

#define ERROR_CUSTOMER_NOT_EXISTS   "O cliente não existe na lista.\n"
#define ERROR_LIST_CUSTOMER_EMPTY        "A lista de clientes está vazia.\n"
#define ERROR_LIST_FULL        "A lista de clientes está cheia.\n"
#define ERROR_CUSTOMER_EXISTS       "O número de cliente já se encontra atribuído.\n"

    //nao esta em uso, automaticamente gerado pelo codigo e contador
#define MIN_NUM_CUSTOMER           1
#define MAX_NUM_CUSTOMER           1000
#define MSG_GET_ID_CUSTOMER       "Insira um número de cliente [0-1000]: "

#define MAX_NAME_CUSTOMER          31
#define MSG_GET_CUSTOMER_NAME     "Insira o nome do cliente: "


#define MAX_NIF_CUSTOMER          999999999  
#define MIN_NIF_CUSTOMER          100000000
#define MSG_GET_NIF               "Insira o nif: "   

#define MAX_COUNTRY_CUSTOMER      20
#define MSG_GET_COUNTRY             "Insira o país: "

#define MAX_CITY_CUSTOMER         20
#define MSG_GET_CITY              "Insira a cidade: "

#define MAX_STREET_CUSTOMER       30
#define MSG_GET_STREET             "Insira a rua: "

#define MAX_NDOOR_CUSTOMER         99999  
#define MIN_NDOOR_CUSTOMER        1
#define MSG_GET_NDOOR               "Insira o número da porta: " 

#define MAX_OPTION                 2    
#define MIN_OPTION                 1
#define MSG_GET_CUSTOMER_OPTION             "Estado do cliente:\n1 - Activo \n2 - Inativo\n "


    //ver necessidade de implementar mais limitadores para os argumentos de customers

#define MIN_DIA                 0
#define MAX_DIA                 31
#define GET_DIA_NASC          "Insira o dia de nascimento: "

#define MIN_MES                 1
#define MAX_MES                 12
#define GET_MES_NASC          "Insira o mês de nascimento: "

#define MIN_ANO                 1990
#define MAX_ANO                 2021
#define GET_ANO_NASC          "Insira o ano de nascimento: "

    typedef struct {
        int id;
        char name[MAX_NAME_CUSTOMER];
        char role[10];
        int nif;
        char country[MAX_COUNTRY_CUSTOMER];

        struct Address {
            char city[MAX_CITY_CUSTOMER];
            char street[MAX_STREET_CUSTOMER];
            int nDoor;

        } address;
        bool isActive;
    } Customer;

    typedef struct {
        Customer* customers;
        int nCustomers, nMax; //contador para quantidade de customers;
    } Customers;

#include "orders.h"
    int checkCustomer(Customers customers, int id);
    int initialiseCustomers(Customers* customers, char* filename);
    void createCustomers(Customers* customers, char* filename);
    void editCustomer(Customers* customers, char* filename, int id);
    void searchCustomer(Customers customers);
    int deleteCustomer(Customers* customers, Orders* orders, char* filename, int id);
    void listCustomers(Customers customers);
    void freeCustomers(Customers* customers);

#endif /* CUSTOMERS_H */



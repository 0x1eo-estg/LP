/**
 * @file orders.h
 * @brief Este header inclui as funcionalidades relativas à gestão de encomendas
*/
#ifndef ORDERS_H
#define ORDERS_H

#define FILE_ORDERS  "orders.bin"

#define INITIAL_ORDERS 5
#define ERROR_ORDER_NOT_EXISTS "A encomenda não existe na lista."
#define ERROR_ORDER_LIST_EMPTY        "A lista de encomendas está vazia."
#define ERROR_PRODS_INACTIVE       "Os produtos estão todos inativos."

#define MIN_NUM_ORDER 0
#define MAX_NUM_ORDER 23

#define MAX_ID_ORDER 7
#define MSG_GET_ID_ORDER "Insira ID da encomenda: "

#define MIN_PROD_ORDER 0
#define MAX_PROD_ORDER 20
#define MSG_GET_QUANTITY_ORDER "Insira a quantidade que deseja para produto n: "

#define NPRODS 3
    typedef struct {
        int day, month, year;
    }Date;


    typedef struct {
        char id[MAX_ID_ORDER];
        int products[NPRODS];
        int total;
        int customerId;
        Date date;
    } Order;

    typedef struct {
        Order* orders;
        int nOrders, nMax;
    } Orders;

#include "products.h"
    
    void freeOrders(Orders* orders);
    int initialiseOrders(Orders* orders, char* filename);
    int checkOrder(Orders orders, char id[MAX_ID_ORDER]);
    void editOrder(Orders* orders, Products* products, char* filename);
    void searchOrder(Orders orders, Products* products);
    void deleteOrder(Orders* orders, char* filename);
    void listOrders(Orders orders, Products* products);
    void createOrders(Orders* orders, Products* products, int customerId, char* filename);
    void viewMyOrders(Orders* orders, Products* products, int customerId);

#endif /* ORDERS_H */



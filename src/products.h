/**
 * @file products.h
 * @brief Este header inclui as funcionalidades relativas à gestão de produtos
*/
#ifndef PRODUCTS_H
#define PRODUCTS_H


#define MAX_PRODUCTS 2048 // maximum number of products that can be stored
#define INITIAL_PRODUCTS 3
#define FILE_PRODS "products.bin"

#define ERROR_PRODUCT_NOT_EXISTS "O producto não existe na lista."
#define ERROR_LIST_PRODUCT_EMPTY "A lista de productos está vazia."

#define MIN_NUM_PRODUCT 0
#define MAX_NUM_PRODUCT 3

#define MAX_ID_SZ 7
#define MSG_GET_ID_PRODUCT "Insira ID de produto: "

#define MAX_NAME_SZ 128
#define MSG_GET_PRODUCT_NAME "Insira o nome do produto: "

#define MAX_DIM_PRODUCT 32
#define MSG_GET_DIM "Insira as dimensoes do produto: "

#define MIN_PROD_PRICE 0
#define MAX_PROD_PRICE 8192
#define MSG_GET_PRICE "Insira o preco: "

#define MAX_OPTION 2
#define MIN_OPTION 1
#define MSG_GET_PRODUCT_OPTION "Estado do producto: \n1 - Activo \n2 - Inativo\n "

typedef struct {
  char id[MAX_ID_SZ];
  char name[MAX_NAME_SZ];
  char dimensions[32];
  int price;
  bool isActive;
} Product;

typedef struct {
  Product* products;
  int nProds, nMax;
} Products;

#include "orders.h"
#include "materials.h"
int initialiseProducts(Products* products, char* filename);
void editProduct(Products* products, char* filename);
void searchProduct(Products products);
void deleteProduct(Products* products, Orders* orders, Materials* materials, char* filename);
void listProducts(Products products);
void freeProducts(Products* products);

#endif

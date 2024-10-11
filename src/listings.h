/**
 * @file listings.h
 * @brief Este header inclui as funcionalidades relativas à criação e de listagens
*/
#ifndef LISTINGS_H
#define LISTINGS_H

#include "customers.h"
#include "products.h"
#include "materials.h"
#include "orders.h"

typedef struct {
    char city[MAX_CITY_CUSTOMER];
    int counter;
}City;
typedef struct {
    City* cities;
    int nCities;
}Cities;

void mostOrdersCustomer(Orders orders, Customers* customers);
void ordersPerCityDate(Orders* orders, Customers* customers);
void cityOrders(Orders* orders, Customers* customers);
void mostOrdersProduct(Orders orders, Products products);
void moneyEarned(Orders orders, Products products);
void notYetDelivered(Orders orders, Products* products);


#endif
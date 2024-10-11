/**
 * @file file.h
 * @brief Este header inclui as funcionalidades relativas à manipulação e gestão de ficheiros
*/
#ifndef FILE_H
#define FILE_H

#include "customers.h"
#include "products.h"
#include "materials.h"
#include "orders.h"
#include "production.h"
#include "listings.h"


void updateNCustomers(int nCustomers, char* filename);
void updateNProducts(int nProds, char* filename);
void updateNMaterials(int nMaterials, char* filename);
void insertCustomer(Customers* customers, char* filename);
void insertProduct(Products* products, char* filename);
void insertMaterial(Materials* materials, char* filename);
void updateCustomer(Customers customers, int pos, char* filename);
void updateProduct(Products products, int pos, char* filename);
void updateMaterial(Materials materials, int pos, char* filename);
void removeCustomer(Customers customers, char* filename);
void removeProduct(Products products, char* filename);
void removeMaterial(Materials materials, char* filename);

void updateNOrders(int nOrders, char* filename);
void updateOrder(Orders orders, int pos, char* filename);
void insertOrder(Orders* orders, char* filename);
void removeOrder(Orders orders, char* filename);
void saveProduction(Production* production, Materials* materials, Date start, Date end, char* filename);
void saveCitiesOrders(Cities cities, Date start, Date end, char* filename);
void saveListOfCities(Cities cities, char* filename);

#endif
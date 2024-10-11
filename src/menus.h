/**
 * @file menus.h
 * @brief Este header inclui as funcionalidades relativas aos menus mostrados ao utilizador
*/
#ifndef MENUS_H
#define MENUS_H

#define ADMIN_PASSWORD "admin"
#define MSG_GET_PASSWORD "Password: "

#include "products.h"
#include "materials.h"
#include "orders.h"


    int useMenu(Customers* customers, Products* products, Materials* materials, Orders* orders);
    void useCustomerMenu(Customers* customers, Products* products, Orders* orders, int id);
    void useAdminMenu(Customers* customers, Products* products, Materials* materials, Orders* orders, int id);

#endif /* MENUS_H */


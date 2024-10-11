#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "input.h"
#include "products.h"
#include "customers.h"
#include "materials.h"
#include "file.h"
#include "menus.h"
#include "listings.h"

int main(void) {
    Customers customers;
    Products products;
    Materials materials;
    Orders orders;

    int option;

    if (initialiseCustomers(&customers, FILE_CUSTOMERS) && initialiseProducts(&products, FILE_PRODS)
        && initialiseMaterials(&materials, FILE_MATERIALS) && initialiseOrders(&orders, FILE_ORDERS)) {
        
        useMenu(&customers, &products, &materials, &orders);

        return 0;

    }
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"
#include "file.h"

//-------CUSTOMER--------
/** Esta função atualiza o número de clientes no ficheiro
 * @param nCustomers
 * @param filename
*/
void updateNCustomers(int nCustomers, char* filename) {
    FILE* fp = fopen(filename, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&nCustomers, sizeof(int), 1, fp);
    fclose(fp);
}

/** Esta função adiciona um cliente no ficheiro
 * @param customers
 * @param filename
*/
void insertCustomer(Customers* customers, char* filename) {
    // update counter
    updateNCustomers(customers->nCustomers, filename);
    FILE* fp = fopen(filename, "ab");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&customers->customers[customers->nCustomers - 1], sizeof(Customer), 1, fp);
    fclose(fp);
}

/** Esta função atualiza os dados de um cliente no ficheiro
 * @param customers
 * @param pos
 * @param filename
*/
void updateCustomer(Customers customers, int pos, char* filename) {
    FILE* fp = fopen(filename, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof(int), SEEK_SET);
    fseek(fp, sizeof(Customer) * pos, SEEK_CUR);
    fwrite(&customers.customers[pos], sizeof(Customer), 1, fp);
    fclose(fp);
}

/** Esta função remove os dados de um cliente no ficheiro
 * @param customers
 * @param filename
*/
void removeCustomer(Customers customers, char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&customers.nCustomers, sizeof(int), 1, fp);
    fwrite(customers.customers, sizeof(Customer), customers.nCustomers, fp);
    fclose(fp);
}
//---------Products
/** Esta função atualiza o número de produtos no ficheiro
 * @param nProducts
 * @param filename
*/
void updateNProducts(int nProducts, char* filename) {
    FILE* fp = fopen(filename, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&nProducts, sizeof(int), 1, fp);
    fclose(fp);
}

/** Esta função atualiza os dados de um produto no ficheiro
 * @param products
 * @param pos
 * @param filename
*/
void updateProduct(Products products, int pos, char* filename) {
    FILE* fp = fopen(filename, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof(int), SEEK_SET);
    fseek(fp, sizeof(Product) * pos, SEEK_CUR);
    fwrite(&products.products[pos], sizeof(Product), 1, fp);
    fclose(fp);
}

/** Esta função adiciona um produto no ficheiro
 * @param products
 * @param filename
*/
void insertProduct(Products* products, char* filename) {
    // update counter
    updateNProducts(products->nProds, filename);
    FILE* fp = fopen(filename, "ab");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&products->products[products->nProds - 1], sizeof(Product), 1, fp);
    fclose(fp);
}

/** Esta função remove os dados de um produto no ficheiro
 * @param products
 * @param filename
*/
void removeProduct(Products products, char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&products.nProds, sizeof(int), 1, fp);
    fwrite(products.products, sizeof(Product), products.nProds, fp);
    fclose(fp);
}

//---------Materials
/** Esta função atualiza o número de materiais no ficheiro
 * @param nMaterials
 * @param filename
*/
void updateNMaterials(int nMaterials, char* filename) {
    FILE* fp = fopen(filename, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&nMaterials, sizeof(int), 1, fp);
    fclose(fp);
}

/** Esta função atualiza os dados de um mateiral no ficheiro
 * @param materials
 * @param pos
 * @param filename
*/
void updateMaterial(Materials materials, int pos, char* filename) {
    FILE* fp = fopen(filename, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof(int), SEEK_SET);
    fseek(fp, sizeof(Material) * pos, SEEK_CUR);
    fwrite(&materials.materials[pos], sizeof(Material), 1, fp);
    fclose(fp);
}

/** Esta função adiciona um material no ficheiro
 * @param materials
 * @param filename
*/
void insertMaterial(Materials* materials, char* filename) {
    //update counter 
    updateNMaterials(materials->nMaterials, filename);
    FILE* fp = fopen(filename, "ab");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&materials->materials[materials->nMaterials - 1], sizeof(Material), 1, fp);
    fclose(fp);
}

/** Esta função remove os dados de um material no ficheiro
 * @param materials
 * @param filename
*/
void removeMaterial(Materials materials, char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&materials.nMaterials, sizeof(int), 1, fp);
    fwrite(materials.materials, sizeof(Material), materials.nMaterials, fp);
    fclose(fp);
}
//---------Orders
/** Esta função atualiza o número de encomendas no ficheiro
 * @param nOrders
 * @param filename
*/
void updateNOrders(int nOrders, char* filename) {
    FILE* fp = fopen(filename, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&nOrders, sizeof(int), 1, fp);
    fclose(fp);
}

/** Esta função atualiza os dados de uma encomanda no ficheiro
 * @param orders
 * @param pos
 * @param filename
*/
void updateOrder(Orders orders, int pos, char* filename) {
    FILE* fp = fopen(filename, "r+b");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fp, sizeof(int), SEEK_SET);
    fseek(fp, sizeof(Order) * pos, SEEK_CUR);
    fwrite(&orders.orders[pos], sizeof(Order), 1, fp);
    fclose(fp);
}

/** Esta função adiciona uma encomenda no ficheiro
 * @param orders
 * @param filename
*/
void insertOrder(Orders* orders, char* filename) {
    //update counter 
    updateNOrders(orders->nOrders, filename);
    FILE* fp = fopen(filename, "ab");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&orders->orders[orders->nOrders - 1], sizeof(Order), 1, fp);
    fclose(fp);
}

/** Esta função remove os dados de uma encomenda no ficheiro
 * @param orders
 * @param filename
*/
void removeOrder(Orders orders, char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&orders.nOrders, sizeof(int), 1, fp);
    fwrite(orders.orders, sizeof(Order), orders.nOrders, fp);
    fclose(fp);
}

//---------------Production
/** Esta função guarda a lista gerada para um ficheiro à escolha do user
 * @param production
 * @param materials
 * @param start
 * @param end
 * @param filename
*/
void saveProduction(Production* production, Materials* materials, Date start, Date end, char* filename) {
    int i = 0;
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "Lista de Produção (%02d/%02d/%d - %02d/%02d/%d)\n", start.day,
        start.month, start.year, end.day, end.month, end.year);
    fprintf(fp, "ID do Material - Descrição do Material - Quantidade - Unidade de medida\n");
    fputs("----------------------------------------------------------------", fp);
    for (i = 0; i < materials->nMaterials; i++) {
        fprintf(fp, "\n%s  %s --- %d  %s\n", materials->materials[i].id, materials->materials[i].description,
            production->materials[i], materials->materials[i].unit);
    }
    fputs("----------------------------------------------------------------", fp);
    fclose(fp);
}

//listagens
/** Esta função guarda a lista de cidades(por data) gerada para um ficheiro à escolha do user
 * @param cities
 * @param start
 * @param end
 * @param filename
*/
void saveCitiesOrders(Cities cities, Date start, Date end, char* filename) {
    int i = 0;
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "Listagem de cidades para entrega de encomendas (%02d/%02d/%d - %02d/%02d/%d)\n", start.day,
        start.month, start.year, end.day, end.month, end.year);
    fprintf(fp, "Cidade ---- Quantidade de encomendas\n");
    fprintf(fp, "----------------------------------------------------------------");
    for (int i = 0; i < cities.nCities; i++) {
        fprintf(fp, "\n%s ----- %d\n", cities.cities[i].city, cities.cities[i].counter);
    }
    fputs("----------------------------------------------------------------", fp);
    fclose(fp);
}

/** Esta função guarda a lista de cidades gerada para um ficheiro à escolha do user
 * @param cities
 * @param filename
*/
void saveListOfCities(Cities cities, char* filename) {
    int i = 0;
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "Listagem de cidades com encomendas registradas\n");
    fprintf(fp, "Cidade ---- Quantidade de encomendas\n");
    fprintf(fp, "----------------------------------------------------------------");
    for (int i = 0; i < cities.nCities; i++) {
        fprintf(fp, "\n%s ----- %d\n", cities.cities[i].city, cities.cities[i].counter);
    }
    fputs("----------------------------------------------------------------", fp);
    fclose(fp);
}
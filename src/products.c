#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "products.h"
#include "input.h"
#include "file.h"

/**
 * Esta função liberta a memória alocada para os products
 * @param products
 */
void freeProducts(Products* products) {
    free(products->products);
}

/**
 * Esta função realoca a memória alocada dos produtos para que seja possivel adicionar mais um produto
 * @param products
 */
void reallocProducts(Products* products) {
    Product* tempPtr = (Product*)realloc(products->products, sizeof(Product) * (products->nMax + 1));
    // if the temporary pointer is not null,
    // copy it to the original pointer and expand its capacity
    if (tempPtr != NULL) {
        products->nMax += 1;
        products->products = tempPtr;
    }
}

/**
 * Esta função dá output dos dados de um produto, passado pelos parâmetros
 * @param product
 */
void printProducts(Product product) {
    printf("\n%s, %s, %s, %d€, %s\n", product.id, product.name, product.dimensions, product.price, product.isActive ? "Ativo" : "Inativo");
}

/**
 * Esta função verifica se já existem produtos. Se existirem é alocada memória suficiente para a quantidade de produtos existente. Se não existirem, é alocada memória para a quantidade inicial de produtos. WIP
 * @param products
 * @param filename
 * @return confirmação se a memória foi alocada ou não
 */
int initialiseProducts(Products* products, char* filename) {
    int confirm = 0;
    FILE* fp = fopen(filename, "rb");
    if (fp != NULL) {
        fread(&products->nProds, sizeof(int), 1, fp);
        if (products->nProds > 0) {
            products->products = (Product*)malloc(products->nProds * sizeof(Product));
            // read info from file
            fread(products->products, sizeof(Product), products->nProds, fp);

            products->nMax = products->nProds;
            confirm = 1;
        }
        fclose(fp);
    }
    if (!confirm) {
        fp = fopen(filename, "wb");
        if (fp != NULL) {
            products->products = (Product*)malloc(INITIAL_PRODUCTS * sizeof(Product));
            products->nProds = 0;
            products->nMax = INITIAL_PRODUCTS;

            confirm = 1;
        }
    }

    return confirm;
}

/**
 * Esta função limpa/zera os valores de um product
 * @param product
 */
void deleteProductData(Product* product) {
    strcpy(product->id, "");
    strcpy(product->name, "");
    strcpy(product->dimensions, "");
    product->price = 0;
    product->isActive = false;
}

/**
 * Esta função verifica se o produto existe, a partir do id
 * @param products
 * @param id
 * @return a posição do produto na lista. Se não existir retorna -1
 */
int checkProduct(Products products, char id[MAX_ID_SZ]) {
    int i = 0;

    while (i < products.nProds) {
        if (strcmp(products.products[i].id, id) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

/**
 * Esta função verifica se o produto existe. Se o produto existir,
 * é pedido ao utilizador novos dados para o produto, e uma confirmação para sobrepor os dados novos na base de dados. Se não existir, é mostrado um erro.
 * @param products
 * @param filename
 */
void editProduct(Products* products, char* filename) {
    int i = 0;
    char id[MAX_ID_SZ];
    readString(id, MAX_ID_SZ, MSG_GET_ID_PRODUCT);
    int pos = checkProduct(*products, id);

    if (pos != -1) {
        readString(products->products[pos].name, MAX_NAME_SZ, MSG_GET_PRODUCT_NAME);
        readString(products->products[pos].dimensions, MAX_DIM_PRODUCT, MSG_GET_DIM);
        products->products[pos].price = getInt(MIN_PROD_PRICE, MAX_PROD_PRICE, MSG_GET_PRICE);

        int option = getInt(MIN_OPTION, MAX_OPTION, MSG_GET_PRODUCT_OPTION);
        switch (option) {
        case 1:
            products->products[pos].isActive = true;
            break;
        case 2:
            products->products[pos].isActive = false;
            break;
        }
        // file save
        int userConfirmInput = getInt(1, 2, "Deseja guardar as alterações na Base de Dados?\n 1 - sim\n 2 - não\n");
        if (userConfirmInput == 1) {
            updateProduct(*products, pos, filename);
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_PRODUCT_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);

    }
}

/**
 * Esta função pede um id de produto ao utilizador, que é verificado se existe.
 * Se existir, é mostrado o produto no terminal. Se não existir mostra um erro.
 * @param products
 */
void searchProduct(Products products) {
    char id[MAX_ID_SZ];
    readString(id, MAX_ID_SZ, MSG_GET_ID_PRODUCT);
    int pos = checkProduct(products, id);

    if (pos != -1) {
        printf("ID - Nome do produto - Dimensões - Preço - Estado\n");
        printProducts(products.products[pos]);
        printf("\n");
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_PRODUCT_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}

/**
 * Esta função valida se existem produtos. Se existirem, são listados os produtos todos e, se não existirem é mostrado um erro
 * @param products
 */
void listProducts(Products products) {
    if (products.nProds > 0) {
        printf("ID - Nome do produto - Dimensões - Preço - Estado\n");
        for (int i = 0; i < products.nProds; i++) {
            printProducts(products.products[i]);
        }
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
 * Esta função verifica se o Produto escolhido está incluido em alguma encomenda da lista de encomendas
 * @param orders apontador para a estrutura de encomendas
 * @param products
 * @param pos
 * @return existência do Produto escolhido está incluido em alguma encomenda da lista de encomendas
 */
int checkProductOrders(Orders* orders, Products* products, int pos) {
    int i = 0;

    while (i < orders->nOrders) {
        if (orders->orders[i].products[pos] > 0) {
            return 1;
        }
        i++;
    }
    return -1;
}

/**
 * Esta função faz o realloc da estrutura de produtos após eliminar um produto
 * @param products
 */
void reallocProductsAfterDelete(Products* products) {
    Product* tempPtr = (Product*)realloc(products->products, sizeof(Product) * (products->nMax - 1));
    // if the temporary pointer is not null,
    // copy it to the original pointer and expand its capacity
    if (tempPtr != NULL) {
        products->nMax -= 1;
        products->products = tempPtr;
    }
}

/**
 * Esta função pede ao utilizador um id e verifica se existe um produto com esse id.
 * Se existir,é verificado se existem encomendas com esse produto para inativar apenas o produto depois da confirmação do utilizador.
 * Se existir e não houverem encomendas com o produto, é pedida confirmação ao utilizador para eliminar o produto.
 * Se não existir, é mostrado um erro.
 * @param products
 * @param orders
 * @param materials
 * @param filename
 */
void deleteProduct(Products* products, Orders* orders, Materials* materials, char* filename) {
    int i = 0;
    char id[MAX_ID_SZ];
    readString(id, MAX_ID_SZ, MSG_GET_ID_PRODUCT);
    int pos = checkProduct(*products, id);

    if (pos != -1) {
        int check = checkProductOrders(orders, products, pos);
        int userConfirmInput = 0;
        if (check == 1) {
            userConfirmInput = getInt(1, 2, "Confirma que deseja alterar o estado do produto para inativo?\n 1 - sim\n 2 - não\n");
            if (userConfirmInput == 1) {
                products->products[pos].isActive = false;
                printf("O produto vai ser apenas inativado pois existem encomendas registadas com o produto.\n");
                updateProduct(*products, pos, filename);
            }
        } else {
            userConfirmInput = getInt(1, 2, "Confirma que deseja eliminar o producto?\n 1 - sim\n 2 - não");
            if (userConfirmInput == 1) {
                for (i = pos; i < products->nProds - 1; i++) {
                    products->products[i] = products->products[i + 1];

                    for (int j = 0; j < materials->nMaterials; j++) {
                        materials->materials[j].quantity[i] = materials->materials[j].quantity[i + 1];
                        updateMaterial(*materials, j, FILE_MATERIALS);
                    }
                    for (int j=0; j<orders->nOrders;j++){
                        orders->orders[j].products[i] =  orders->orders[j].products[i+1];
                        updateOrder(*orders, j, FILE_ORDERS);
                    }
                }
                deleteProductData(&products->products[i]);
                for (int j = 0; j < materials->nMaterials; j++) {
                    materials->materials[j].quantity[i] = 0;
                    updateMaterial(*materials, j, FILE_MATERIALS);
                }
                 for (int j = 0; j < materials->nMaterials; j++) {
                    orders->orders[j].products[i]=0;
                    updateOrder(*orders, j, FILE_ORDERS);
                }
                
                products->nProds--;
                removeProduct(*products, FILE_PRODS);
                printf("Produto apagado com sucesso.\n");
                reallocProductsAfterDelete(products);
            }
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_PRODUCT_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}
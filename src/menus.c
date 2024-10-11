#include <stdio.h>
#include "customers.h"
#include "input.h"
#include "menus.h"
#include <string.h>
#include "products.h"
#include "materials.h"
#include "orders.h"
#include "file.h"
#include "listings.h"
#include "production.h"

/**
 * Esta função pede e verifica a  password do Administrador
 * @return confirmação se a password está correta ou errada
 * */
int getPassword() {
    char password[15];
    int tries = 0;

    while (tries < 3) {
        readString(password, 15, MSG_GET_PASSWORD);
        if (strcmp(password, ADMIN_PASSWORD) == 0) {
            return 1;
        } else {
            tries++;
            printf("Password Inválida (%d/3 tentativas)\n", tries);
        }
    }
    return 0;
}

/**
 * Esta função gere e mostra o menu de gestão de materiais
 * @param materials
 */
void manageMaterials(Materials* materials) {
    int prompt = -1;

    do {
        clearConsole();
        printf("|         ------------------ Gestão de Materiais ------------------       |\n");
        printf("|   1 - Listar materiais                                                  |\n");
        printf("|   2 - Editar materiais                                                  |\n");
        printf("|   0 - Voltar                                                            |\n");
        printf("|   Materiais: %d                                                         |\n", materials->nMaterials);
        printf("| ----------------------------------------------------------------------- |\n");

        prompt = getInt(0, 2, "");
        char c = '0';

        switch (prompt) {
            case 1:
                clearConsole();
                printf("-----------------------------------------------------------------------");
                listMaterials(*materials);
                puts(" ----------------------------------------------------------------------- ");
                printf("Pressione a tecla «ENTER» para sair desta lista!");
                scanf("%c", &c);
                break;
            case 2:
                clearConsole();
                editMaterial(materials, FILE_MATERIALS);
                break;
            case 0:
                clearConsole();
                break;
        }
    } while (prompt != 0);
}

// management menus

/**
 * Esta função diz respeito ao menu de gestão de clientes
 * @param customers apontador para estrutura de clientes
 * @param orders apontador para estrutura de encomendas
 * @param id identificação do Admin
 * */
void manageCustomers(Customers* customers, Orders* orders, int id) {
    int prompt = -1;
    do {
        clearConsole();
        printf("|         ------------------ Gestão de Clientes ------------------        |\n");
        printf("|   1 - Listar Clientes                                                   |\n");
        printf("|   2 - Criar cliente                                                     |\n");
        printf("|   3 - Editar cliente                                                    |\n");
        printf("|   4 - Procurar cliente                                                  |\n");
        printf("|   5 - Deletar cliente                                                   |\n");
        printf("|   0 - Voltar                                                            |\n");
        printf("|   Clientes: %d/%d                                                       |\n", customers->nCustomers, customers->nMax);
        printf("| ----------------------------------------------------------------------- |\n");

        prompt = getInt(0, 5, "");
        char c = '0';
        switch (prompt) {
            case 1:
                clearConsole();

                printf(" ----------------------------------------------------------------------- ");
                listCustomers(*customers);
                puts(" ----------------------------------------------------------------------- ");
                printf("Pressione a tecla «ENTER» para sair desta lista!");
                scanf("%c", &c);
                break;
            case 2:
                clearConsole();
                createCustomers(customers, FILE_CUSTOMERS);
                break;
            case 3:
                clearConsole();
                editCustomer(customers, FILE_CUSTOMERS, id);
                break;
            case 4:
                clearConsole();
                printf(" ----------------------------------------------------------------------- \n");
                searchCustomer(*customers);
                puts(" ----------------------------------------------------------------------- ");
                printf("Pressione a tecla «ENTER» para sair desta lista!");
                scanf("%c", &c);
                break;

            case 5:
                clearConsole();
                deleteCustomer(customers, orders, FILE_CUSTOMERS, id);
                break;
            case 0:
                clearConsole();
                break;
        }
    } while (prompt != 0);
}

/**
 * Esta função diz respeito ao menu de gestão de produtos
 * @param products apontador para estrutura de produtos
 * */
void manageProducts(Products* products, Orders* orders, Materials* materials) {
    int prompt = -1;
    do {
        clearConsole();
        printf("|         ------------------ Gestão de Produtos ------------------        |\n");
        printf("|   1 - Listar produtos                                                   |\n");
        printf("|   2 - Editar produto                                                    |\n");
        printf("|   3 - Remover produto                                                   |\n");
        printf("|   4 - Procurar produto                                                  |\n");
        printf("|   Produtos: %d/%d                                                       |\n", products->nProds, products->nMax);
        printf("|   0 - Voltar                                                            |\n");
        printf("| ----------------------------------------------------------------------- |\n");
        char c = '0';
        prompt = getInt(0, 4,"");

        switch (prompt) {
            case 1:
                clearConsole();
                printf(" ----------------------------------------------------------------------- \n");
                // fazer print de o que cada valor se trata (id, nome...)
                listProducts(*products);
                puts(" ----------------------------------------------------------------------- ");
                printf("Pressione a tecla «ENTER» para sair desta lista!");
                scanf("%c", &c);
                break;
            case 2:
                clearConsole();
                editProduct(products, FILE_PRODS);
                break;
            case 3:
                clearConsole();
                deleteProduct(products, orders, materials, FILE_PRODS);
                break;
            case 4:
                clearConsole();
                printf(" ----------------------------------------------------------------------- \n");
                searchProduct(*products);
                puts(" ----------------------------------------------------------------------- ");
                printf("Pressione a tecla «ENTER» para sair desta lista!");
                scanf("%c", &c);
                break;
        }
    } while (prompt != 0);
}

/**
 * Esta função diz respeito ao menu de gestão de encomendas
 * @param orders apontador para estrutura de encomendas
 * @param products apontador para estrutura de produtos
 * */
void manageOrders(Orders* orders, Products* products) {
    int prompt = -1;
    do {
        clearConsole();
        printf("|         ----------------- Gestão de Encomendas -----------------        |\n");
        printf("|   1 - Listar encomendas                                                 |\n");
        printf("|   2 - Editar encomenda                                                  |\n");
        printf("|   3 - Eliminar encomenda                                                |\n");
        printf("|   4 - Procurar encomenda                                                |\n");
        printf("|   Encomendas: %d/%d                                                     |\n", orders->nOrders, orders->nMax);
        printf("|   0 - Voltar                                                            |\n");
        printf("| ----------------------------------------------------------------------- |\n");

        prompt = getInt(0, 4, "");
        char c = '0';
        switch (prompt) {
            case 1:
                clearConsole();
                printf(" ----------------------------------------------------------------------- \n");
                listOrders(*orders, products);
                puts(" ----------------------------------------------------------------------- ");
                printf("Presione a tecla «ENTER» para sair desta lista!");
                scanf("%c", &c);
                break;
            case 2:
                clearConsole();
                editOrder(orders, products, FILE_ORDERS);
                break;
            case 3:
                clearConsole();
                deleteOrder(orders, FILE_ORDERS);
                break;
            case 4:
                clearConsole();
                printf(" ----------------------------------------------------------------------- \n");
                searchOrder(*orders, products);
                puts(" ----------------------------------------------------------------------- ");
                printf("Pressione a tecla «ENTER» para sair desta lista!");
                scanf("%c", &c);
                break;
        }
    } while (prompt != 0);
}

/**
 * Esta função diz respeito ao menu de gestão de listagens
 * @param customers apontador para estrutura de clientes
 * @param products apontador para estrutura de produtos
 * @param materials apontador para estrutura de materials
 * @param orders apontador para estrutura de encomendas
 * */
void manageLists(Customers* customers, Products* products, Materials* materials, Orders* orders) {
    int prompt = -1;
    do {
        clearConsole();
        printf("|               ---------------- Listagens ----------------               |\n");
        printf("|   1 - Cidades para entrega de encomendas em determinada data            |\n");
        printf("|   2 - Cliente com maior número de encomendas                            |\n");
        printf("|   3 - Produtos ordenados por quantidade de encomendas                   |\n");
        printf("|   4 - Cidades com encomendas registadas                                 |\n");
        printf("|   5 - Encomendas por entregar                                           |\n");
        printf("|   6 - Faturamento                                                       |\n");
        printf("|   0 - Voltar                                                            |\n");
        printf("| ----------------------------------------------------------------------- |\n");

        prompt = getInt(0, 6, "");
        char c = '0';
        switch (prompt) {
            case 1:
                clearConsole();
                ordersPerCityDate(orders, customers);
                break;
            case 2:
                clearConsole();
                mostOrdersCustomer(*orders, customers);
                break;
            case 3:
                clearConsole();
                mostOrdersProduct(*orders,*products);
                break;
            case 4:
                clearConsole();
                cityOrders(orders, customers);
                break;
            case 5:
                clearConsole();
                notYetDelivered(*orders, products);
                break;
            case 6:
                clearConsole();
                moneyEarned(*orders,*products);
                break;      
        }
    } while (prompt != 0);

}

// End of management menus


// customers menu

/**
 * Esta função diz respeito ao menu de clientes
 * @param customers apontador para estrutura de clientes
 * @param id identificação do cliente
 * */
void useCustomerMenu(Customers* customers, Products* products, Orders* orders, int id) {
    int prompt = -1;
    do {

        clearConsole();
        printf("| ------------------------------------------------------------------ |\n");
        printf("|          --------------- Menu de Clientes ---------------          |\n");
        printf("|   1 - Editar cliente                                               |\n"); // edit and delete customer that is logged in
        printf("|   2 - Registar encomenda                                           |\n");
        printf("|   3 - Ver minhas encomendas                                        |\n"); // create  function that associates and prints customers and
        printf("|   4 - Deletar cliente                                              |\n");
        printf("|   0 - Sair                                                         |\n");
        printf("| ------------------------------------------------------------------ |\n");

        prompt = getInt(0, 4, "");
        char c = '0';
        switch (prompt) {
            case 1:
                clearConsole();
                editCustomer(customers, FILE_CUSTOMERS, id);

                break;
            case 2:
                clearConsole();
                createOrders(orders, products, id, FILE_ORDERS);

                break;
            case 3:
                clearConsole();
                printf(" ----------------------------------------------------------------------- \n");
                viewMyOrders(orders, products, id);
                puts(" ----------------------------------------------------------------------- ");
                printf("Presione a tecla «ENTER» para sair desta lista!");
                scanf("%c", &c);
                break;
            case 4:
                clearConsole();
                int confirm = deleteCustomer(customers, orders, FILE_CUSTOMERS, id);
                //if user was deleter, logoff and exit out of menu Cliente
                if (confirm == 1) {
                    prompt = 0;
                }
                break;
        }
    } while (prompt != 0);
}

// admin menu

/**
 * Esta função diz respeito ao menu do Administrador
 * @param customers apontador para estrutura de clientes
 * @param products apontador para a estrutura de produtos
 * @param materials apontador para a estrutura de materiais
 * @param orders apontador para a estrutura de encomendas
 * @param id identificação do Admin
 * */
void useAdminMenu(Customers* customers, Products* products, Materials* materials, Orders* orders, int id) {
   
    int prompt = -1;
    do {
        clearConsole();
        printf("| ----------------------------------------------------------------------- |\n");
        printf("|           ------------------ Menu de Admin ------------------           |\n");
        printf("|   1 - Gestão de Clientes                                                |\n");
        printf("|   2 - Gestão de Encomendas                                              |\n");
        printf("|   3 - Gestão de Produtos                                                |\n");
        printf("|   4 - Gestão de Materiais                                               |\n");
        printf("|   5 - Gestão de Produção                                                |\n");
        printf("|   6 - Listagens/Relatórios                                              |\n");
        printf("|   0 - Sair                                                              |\n");
        printf("| ----------------------------------------------------------------------- |\n");
        prompt = getInt(0, 6, "");

        switch (prompt) {
            case 1:
                clearConsole();
                manageCustomers(customers, orders, id);
                break;
            case 2:
                clearConsole();
                manageOrders(orders, products);
                break;
            case 3:
                clearConsole();
                manageProducts(products, orders, materials);
                break;
            case 4:
                clearConsole();
                manageMaterials(materials);
                break;

            case 5:
                clearConsole();
                generateProduction(orders, materials);
                break;
            case 6:
                clearConsole();
                manageLists(customers, products, materials, orders);
                break;
        }
    } while (prompt != 0);
}


// First menu

/**
 * Esta função diz respeito ao menu de login
 * @param customers apontador para estrutura de clientes
 * @param products apontador para estrutura de produtos
 * */
int useMenu(Customers* customers, Products* products, Materials* materials, Orders* orders) {
    int prompt = -1;
    do {
        clearConsole();
        // 0 = no ID  1001 = Admin
        prompt = getInt(0, 1002,
                " ---------------------------- Insira o seu ID -------------------------- \n"
                " Caso não possua ID, introduza 0(zero) ------------------- 1002 (Fechar) \n");

        switch (prompt) {
            case 0:
                clearConsole();
                createCustomers(customers, FILE_CUSTOMERS);
                break;
            case 1001: // define user for admin
                clearConsole();
                if (getPassword()) {
                    useAdminMenu(customers, products, materials, orders, prompt);
                }
                break;
            default:
                clearConsole();
                // check if user exists
                if (checkCustomer(*customers, prompt) != -1) {
                    // show menus if user actually exists
                    useCustomerMenu(customers, products, orders, prompt);
                    // case ID doesnt exists, ask for ID again
                    // printf(ERROR_CUSTOMER_NOT_EXISTS);  
                }
                break;
            case 1002: // close
                freeCustomers(customers);
                freeProducts(products);
                freeMaterials(materials);
                freeOrders(orders);
                return 1;
                break;
        }
    } while (prompt != 1002);
}

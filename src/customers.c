#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "customers.h"
#include "input.h"
#include "file.h"

/**
 * Esta função liberta a memória alocada para os clientes
 * @param customers
 */
void freeCustomers(Customers* customers) {
    free(customers->customers);
}

/**
 * Esta função realoca a memória alocada para os clientes para que seja possivel adicionar mais um cliente
 * @param customers
 */
void reallocCustomers(Customers* customers) {
    Customer* tempPtr = (Customer*)realloc(customers->customers, sizeof(Customer) * (customers->nMax + 1));
    // if the temporary pointer is not null,
    // copy it to the original pointer and expand its capacity
    if (tempPtr != NULL) {
        customers->nMax += 1;
        customers->customers = tempPtr;
    }
}

/**
 * Esta função dá output dos dados de um cliente, passado pelos parâmetros
 * @param customer
 */
void printCustomer(Customer customer) {
    printf("\n%-4d %-10s %-5d %-6s %-6s %-6s %-4d %-3s %-2s\n", customer.id, customer.name,
        customer.nif, customer.country, customer.address.city,
        customer.address.street, customer.address.nDoor, customer.role, customer.isActive ? "Ativo" : "Inativo");
}

/**
 * Esta função verifica se já existem clientes. Se existirem é alocada memória suficiente para a quantidade de clientes existente. Se não existirem, é alocada memória para a quantidade inicial de produtos. WIP
 * @param customers
 * @param filename
 * @return confirmação se a memória foi alocada ou não
 */
int initialiseCustomers(Customers* customers, char* filename) {
    int confirm = 0;

    FILE* fp = fopen(filename, "rb");
    if (fp != NULL) {
        fread(&customers->nCustomers, sizeof(int), 1, fp);
        if (customers->nCustomers > 0) {
            customers->customers = (Customer*)malloc(customers->nCustomers * sizeof(Customer));
            // read info from file
            fread(customers->customers, sizeof(Customer), customers->nCustomers, fp);

            customers->nMax = customers->nCustomers;
            confirm = 1;
        }
        fclose(fp);
    }
    if (!confirm) {
        fp = fopen(filename, "wb");
        if (fp != NULL) {
            customers->customers = (Customer*)malloc(INITIAL_CUSTOMERS * sizeof(Customer));
            customers->nCustomers = 0;
            customers->nMax = INITIAL_CUSTOMERS;

            confirm = 1;
        }
    }
    return confirm;
}

/**
 * Esta função limpa/zera os valores de um cliente
 * @param customer
 */
void deleteCustomerData(Customer* customer) {

    customer->id = 0;
    strcpy(customer->name, "");
    strcpy(customer->role, "");
    customer->nif = 0;
    strcpy(customer->country, "");
    strcpy(customer->address.city, "");
    strcpy(customer->address.street, "");
    customer->address.nDoor = 0;
    customer->isActive = false;
}

/**
 * Esta função verifica se o cliente existe, a partir do id
 * @param customers
 * @param id
 * @return a posição do cliente na lista. Se não existir retorna -1
 */
int checkCustomer(Customers customers, int id) {

    int i = 0;

    while (i < customers.nCustomers) {
        if (customers.customers[i].id == id) {
            return i;
        }
        i++;
    }
    return -1;
}

/**
 * Esta função pede um id ao utilizador a ser verificado se já existe algum cliente com o mesmo id. Se não existir, são pedidos os restantes campos necessários para preenchar uma struct de Customer.
 * @param customers
 * @return o numero total de customers. Se o id ja existir , retorna -1
*/
int createCustomer(Customers* customers) {

    int userIdInput = getInt(MIN_NUM_CUSTOMER, MAX_NUM_CUSTOMER, MSG_GET_ID_CUSTOMER);

    if (checkCustomer(*customers, userIdInput) == -1) {

        // get values for customer
        customers->customers[customers->nCustomers].id = userIdInput; //
        readString(customers->customers[customers->nCustomers].name, MAX_NAME_CUSTOMER, MSG_GET_CUSTOMER_NAME);

        strcpy(customers->customers[customers->nCustomers].role, "Cliente\0");

        customers->customers[customers->nCustomers].nif = getInt(MIN_NIF_CUSTOMER, MAX_NIF_CUSTOMER, MSG_GET_NIF);

        readString(customers->customers[customers->nCustomers].country, MAX_COUNTRY_CUSTOMER, MSG_GET_COUNTRY);

        readString(customers->customers[customers->nCustomers].address.city, MAX_CITY_CUSTOMER, MSG_GET_CITY);
        readString(customers->customers[customers->nCustomers].address.street, MAX_STREET_CUSTOMER, MSG_GET_STREET);
        customers->customers[customers->nCustomers].address.nDoor = getInt(MIN_NDOOR_CUSTOMER, MAX_NDOOR_CUSTOMER, MSG_GET_NDOOR);

        customers->customers[customers->nCustomers].isActive = true;

        // With the add of new customer, incremment in the customer count
        return customers->nCustomers++;
    }

    return -1;
}

/**
 * Esta função realoca a memoria para +1 cliente se o nCustomers = nMax
 * Se o numero de clientes for menor que o máximo de clientes, é verificado se o cliente ja existe.
 * Se o cliente ainda nao existir, é pedida confirmação ao cliente se quer guardar a informaçao na base de dados
 * @param customers
 * @param filename
 * @return o numero total de customers. Se o id ja existir , retorna -1
*/
void createCustomers(Customers* customers, char* filename) {
    if (customers->nCustomers == customers->nMax) {
        reallocCustomers(customers);
    }

    if (customers->nCustomers < customers->nMax) {
        int pos = createCustomer(customers);
        if (pos == -1) {
            char c = '0';
            printf(" ----------------------------------------------------------------------- \n");
            puts(ERROR_CUSTOMER_EXISTS);
            puts(" ----------------------------------------------------------------------- ");
            printf("Pressione a tecla «ENTER» para voltar!");
            scanf("%c", &c);
        } else {
            clearConsole();
            printf("\n ----------------------------------------------------------------------- ");
            printf("\nID - Nome do cliente - NIF - País - Cidade - Morada - Nº Porta - Estado\n");
            printCustomer(customers->customers[pos]);
            puts(" ----------------------------------------------------------------------- ");

            int userConfirmInput = getInt(1, 2, "Deseja guardar as alterações na Base de Dados?\n 1 - sim\n 2 - não\n");
            if (userConfirmInput == 1) {
                insertCustomer(customers, filename);
            } else {
                //delete data
                deleteCustomerData(&customers->customers[pos]);
                if (customers->nCustomers > 0) {
                    customers->nCustomers--;
                }
            }
        }
    }
}

/**
 * Esta função recebe um id nos parametros, Se o id for de algum administrador, permite ao utilizador escolher o id de um cliente para editar.
 * Se o id for de um cliente, são mudados os valores desse mesmo cliente.
 * Os valores são pedidos ao utilizador a partir de prompts.
 * Depois dos campos todos, é pedida uma confirmação ao utilizador para guardar na base de dados.
 * Se o id fornecido não existir, é mostrado um erro no terminal.
 * @param customers
 * @param filename
 * @param id
 */
void editCustomer(Customers* customers, char* filename, int id) {
    int pos = 0;
    if (id == 1001) { // admin can choose which customer to edit
        pos = checkCustomer(*customers, getInt(MIN_NUM_CUSTOMER, MAX_NUM_CUSTOMER, MSG_GET_ID_CUSTOMER));
    } else {
        pos = checkCustomer(*customers, id);
    }

    if (pos != -1) {
        readString(customers->customers[pos].name, MAX_NAME_CUSTOMER, MSG_GET_CUSTOMER_NAME);
        customers->customers[pos].nif = getInt(MIN_NIF_CUSTOMER, MAX_NIF_CUSTOMER, MSG_GET_NIF);
        readString(customers->customers[pos].country, MAX_COUNTRY_CUSTOMER, MSG_GET_COUNTRY);
        readString(customers->customers[pos].address.city, MAX_CITY_CUSTOMER, MSG_GET_CITY);
        readString(customers->customers[pos].address.street, MAX_STREET_CUSTOMER, MSG_GET_STREET);
        customers->customers[pos].address.nDoor = getInt(MIN_NDOOR_CUSTOMER, MAX_NDOOR_CUSTOMER, MSG_GET_NDOOR);

        int option = getInt(MIN_OPTION, MAX_OPTION, MSG_GET_CUSTOMER_OPTION);
        switch (option) {
        case 1:
            customers->customers[pos].isActive = true;
            break;
        case 2:
            customers->customers[pos].isActive = false;
            break;
        }
        // file save
        int userConfirmInput = getInt(1, 2, "Deseja guardar as alterações na Base de Dados?\n 1 - sim\n 2 - não\n");
        if (userConfirmInput == 1) {
            updateCustomer(*customers, pos, filename);
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_CUSTOMER_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}

/**
 * Esta função pede um id de cliente ao utilizador, que é verificado se existe.
 * Se existir, é mostrado o cliente no terminal. Se não existir mostra um erro.
 * @param customers
 */
void searchCustomer(Customers customers) {
    int pos = checkCustomer(customers, getInt(MIN_NUM_CUSTOMER, MAX_NUM_CUSTOMER, MSG_GET_ID_CUSTOMER));

    if (pos != -1) {
        printf("\nID - Nome do cliente - NIF - País - Cidade - Morada - Nº Porta - Estado\n");
        printCustomer(customers.customers[pos]);
        printf("\n");
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_CUSTOMER_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}




/**
 * Esta função verifica se o cliente tem alguma encomenda
 * @param orders
 * @param customerId
 * @return 1 se o cliente tiver encomendas, senão retorna -1
 */
int checkCustomerOrders(Orders* orders, int customerId) {
    int i = 0;

    while (i < orders->nOrders) {

        if (orders->orders[i].customerId == customerId) {
            return 1;
        }
        i++;
    }
    return -1;
}

/**
 * Esta função faz o realloc da estrutura de customers após eliminar um cliente
 * @param customers
 */
void reallocCustomersAfterDelete(Customers* customers) {
    Customer* tempPtr = (Customer*)realloc(customers->customers, sizeof(Customer) * (customers->nMax - 1));
    // if the temporary pointer is not null,
    // copy it to the original pointer and expand its capacity
    if (tempPtr != NULL) {
        customers->nMax -= 1;
        customers->customers = tempPtr;
    }
}

/**
 * Esta função pede ao utilizador um id e verifica se existe um cliente com esse id.
 * Se existir,é verificado se existem encomendas feitas por esse cliente para apenas inativar o cliente depois da confirmação do utilizador.
 * Se existir e não houverem encomendas feitas pelo cliente, é pedida confirmação ao utilizador para eliminar o cliente.
 * Se não existir, é mostrado um erro.
 * @param products
 * @param orders
 * @param materials
 * @param filename
 * @return confirmação se foi eliminado ou não
 */
int deleteCustomer(Customers* customers, Orders* orders, char* filename, int id) {
    int i = 0;
    int pos = 0;
    int customerId = 0;
    if (id == 1001) { // admin can choose which customer to edit
        customerId = getInt(MIN_NUM_CUSTOMER, MAX_NUM_CUSTOMER, MSG_GET_ID_CUSTOMER);
        pos = checkCustomer(*customers, customerId);
    } else {
        customerId = id;
        pos = checkCustomer(*customers, customerId);
    }

    if (pos != -1) {
        int check = checkCustomerOrders(orders, customerId);
        int userConfirmInput = 0;
        if (check == 1) {
            userConfirmInput = getInt(1, 2, "Confirma que deseja Inativar o cliente?\n"
                "1 -Sim\n"
                "2 - Não\n");
            if (userConfirmInput == 1) {
                customers->customers[pos].isActive = false;
                //print  customer incativated because order found

                updateCustomer(*customers, pos, filename);
            }
        } else {
            userConfirmInput = getInt(1, 2, "Confirma que deseja eliminar o cliente?\n"
                "1 -Sim\n"
                "2 - Não\n");

            if (userConfirmInput == 1) {
                // move the array data back to where will be empty
                for (i = pos; i < customers->nCustomers - 1; i++) {
                    customers->customers[i] = customers->customers[i + 1];
                }
                deleteCustomerData(&customers->customers[i]); // i = nCustomers - 1
                customers->nCustomers--;
                // remove in file
                removeCustomer(*customers, FILE_CUSTOMERS);
                //realloc
                reallocCustomersAfterDelete(customers);
                return userConfirmInput;
            }
        }

    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_CUSTOMER_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
    return 0;
}

/**
 * Esta função valida se existem clientes. Se existirem, são listados os clientes todos e, se não existirem é mostrado um erro
 * @param customers
 */
void listCustomers(Customers customers) {
    if (customers.nCustomers > 0) {
        printf("\nID - Nome do cliente - NIF - País - Cidade - Morada - Nº Porta - Estado\n");
        for (int i = 0; i < customers.nCustomers; i++) {
            printCustomer(customers.customers[i]);
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_ORDER_LIST_EMPTY);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}
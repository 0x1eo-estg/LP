#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "materials.h"
#include "input.h"
#include "file.h"

/**
 * Esta função liberta memória alocada para a lista de materiais
 *
 * @param materials apontador para o struct de materiais
 */
void freeMaterials(Materials* materials) {
    free(materials->materials);
}

/**
 * Esta função imprime os dados dum material específico
 *
 * @param material struct de materiais
 */
void printMaterials(Material material) {
    printf("\n%s, %s, %d, %d, %d, %s\n", material.id, material.description,
        material.quantity[0], material.quantity[1], material.quantity[2], material.unit);
}

/**
 * Esta função abre o ficheiro respetivo aos materiais e carrega os dados para o struct
 *
 * @param materials apontador para o struct de materiais
 * @param filename nome do ficheiro que contém a lista de materiais
 * @return sucesso ou não do processo
 */
int initialiseMaterials(Materials* materials, char* filename) {
    int confirm = 0;
    FILE* fp = fopen(filename, "rb");
    if (fp != NULL) {
        fread(&materials->nMaterials, sizeof(int), 1, fp);
        if (materials->nMaterials > 0) {
            materials->materials = (Material*)malloc(materials->nMaterials * sizeof(Material));
            //read info from file
            fread(materials->materials, sizeof(Material), materials->nMaterials, fp);

            //Materials->nMax = Materials->nMaterials;
            confirm = 1;
        }
        fclose(fp);
    }
    if (!confirm) {
        fp = fopen(filename, "wb");
        if (fp != NULL) {
            materials->materials = (Material*)malloc(MAX_NUM_MATERIAL * sizeof(Material));
            materials->nMaterials = 0;
            //Materials->nMax = INITIAL_Materials;

            confirm = 1;
        }
    }
    return confirm;
}

//to be removed
void deleteMaterialData(Material* material) {
    int i = 0;
    strcpy(material->id, "");
    strcpy(material->description, "");
    strcpy(material->unit, "");
    for (i = 0; i < NPRODS; i++) {
        material->quantity[i] = 0;
    }

}

/**
 * Esta função verifica se o material invocado existe
 *
 * @param materials struct de materiais
 * @param id ID do material a ser procurado na lista
 * @return a posição do material, se existir, caso contrário, retorna -1
 */
int checkMaterial(Materials materials, char id[MAX_ID_MATERIAL]) {

    int i = 0;

    while (i < materials.nMaterials) {
        if (strcmp(materials.materials[i].id, id) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

/**
 * Esta função edita os dados de um material do struct
 *
 * @param materials apontador para o struct de materiais
 * @param filename nome do ficheiro que contém a lista de materiais
 */
void editMaterial(Materials* materials, char* filename) {
    int i = 0;
    char id[MAX_ID_MATERIAL];
    readString(id, MAX_ID_MATERIAL, MSG_GET_ID_MATERIAL);
    int pos = checkMaterial(*materials, id);

    if (pos != -1) {
        readString(materials->materials[pos].description, MAX_DESC_MATERIAL, MSG_GET_DESC);
        do {
            readString(materials->materials[pos].unit, MAX_MATERIAL_UNIT, MSG_GET_UNIT);
        } while (strcmp(materials->materials[pos].unit, "UN") != 0 &&
            strcmp(materials->materials[pos].unit, "PAR") != 0);
        for (i = 0; i < NPRODS; i++) {
            materials->materials[pos].quantity[i] = getInt(MIN_QUANTITY, MAX_QUANTITY, MSG_GET_QUANTITY);
        }


        //file save
        int userConfirmInput = getInt(1, 2, "Deseja guardar as alterações na Base de Dados?\n 1 - sim\n 2 - não\n");
        if (userConfirmInput == 1) {
            updateMaterial(*materials, pos, filename);
        }

    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_MATERIAL_NOT_EXISTS);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}

/**
 * Esta função imprime todos os materiais
 *
 * @param materials struct de materiais
 */
void listMaterials(Materials materials) {
    if (materials.nMaterials > 0) {
        int i = 0;
        printf("\nID - Descrição - Prod(1) - Prod(2) - Prod(3) - Unidade\n");
        for (i = 0; i < materials.nMaterials; i++) {
            printMaterials(materials.materials[i]);
        }
    } else {
        char c = '0';
        printf(" ----------------------------------------------------------------------- \n");
        puts(ERROR_MATERIAL_LIST_EMPTY);
        puts(" ----------------------------------------------------------------------- ");
        printf("Pressione a tecla «ENTER» para voltar!");
        scanf("%c", &c);
    }
}




/**
 * @file materials.h
 * @brief Este header inclui as funcionalidades relativas à gestão de materiais
*/
#ifndef MATERIALS_H
#define MATERIALS_H


#define FILE_MATERIALS  "materials.bin"

#define ERROR_MATERIAL_NOT_EXISTS "O materiais não existe na lista."
#define ERROR_MATERIAL_LIST_EMPTY        "A lista de materiais está vazia."


#define MIN_NUM_MATERIAL 0
#define MAX_NUM_MATERIAL 23

#define MAX_ID_MATERIAL 6
#define MSG_GET_ID_MATERIAL "Insira ID de material: "


#define MAX_DESC_MATERIAL 20
#define MSG_GET_DESC "Insira a descrição do material: "


#define MAX_MATERIAL_UNIT 5
#define MSG_GET_UNIT "Insira a Unidade [UN/PAR]: "

#define MAX_QUANTITY                50    
#define MIN_QUANTITY                0
#define MSG_GET_QUANTITY             "Insira a quantidade deste material para o produto "

#define NPRODS 3

typedef struct {
    char id[MAX_ID_MATERIAL];
    char description[MAX_DESC_MATERIAL];
    char unit[MAX_MATERIAL_UNIT];
    int quantity[NPRODS];
} Material;

typedef struct {
    Material* materials;
    int nMaterials;
} Materials;

int initialiseMaterials(Materials* materials, char* filename);
void editMaterial(Materials* materials, char* filename);
void listMaterials(Materials materials);
void freeMaterials(Materials* materials);

#endif


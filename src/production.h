/**
 * @file production.h
 * @brief Este header inclui as funcionalidades relativas à gestão de produção
*/
#ifndef PRODUCTION_H
#define PRODUCTION_H

#include "customers.h"
#define FILE_LENGHT        15
#define NMATS               23

#define MSG_GET_FILENAME   "\nInsira o nome do ficheiro que deseja guardar a lista?\n"

typedef struct {
    int materials[NMATS];
}Production;

#include "orders.h"
#include "materials.h"
int compareDates(Date date, Date start, Date end);
int compareToCurrentDate(Date date);
int validDate(int day, int mon, int year);
void generateProduction(Orders* orders, Materials* materials);

#endif /* PRODUCTION_H */


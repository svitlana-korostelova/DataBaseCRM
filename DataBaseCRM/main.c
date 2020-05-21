//
//  main.c
//  DataBaseCRM
//
//  Created by Svitlana Korostelova on 09.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#include "SQLService.h"

int customersCount;
struct Customer *customers;

int eventsCount;
struct Event *events;


int main(){
    sqlite3 *db;
    char *databaseName = "CRM.db";
    int rc;
    rc = sqlite3_open(databaseName, &db);
    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }
    resetCustomers(); // Выделит память для массива структур для первой загрузки
    
    while (1) {
        int menu = 0;
        printf("\n==========================================\n"
               "Введіть цифру для керування пунктами меню:\n"
               "1 - Первинне завантаження БД з файлу\n"
               "2 - Навігація по звітам\n"
               "3 - Додати нового клієнта\n"
               "4 - Додати нову подію клієнту\n"
               "5 - Видалити запис\n"
               "6 - Редагувати запис\n"
               "7 - Вивантажити БД у файл\n"
               "8 - ВИЙТИ З ПРОГРАМИ\n"
               "==========================================\n");
        fflush(stdin);
        scanf("%i", &menu);
    
        switch(menu){
            case 1: {
    //Требования ко входящим данным при загрузке из файла: для разделения строки на отдельные значения, записываемые в массивы символов нужен разделитель "|" без пробелов, в том числе в конце строки
                accessLoadDataFromFile(db);
                break;
            }
            case 2: {
                showReports(db);
                break;
            }
            case 3: {
                addCustomer(db);
                readCustomers(db);
                printf("Клієнту присвоєно номер картки(ID): %i\n", customers[customersCount-1].id);
                break;
            }
            case 4:{
                addNewEvent(db);
                break;
            }
            case 5:  {
                int id;
                printf("Введіть номер картки(ID), що потребує видалення:\n");
                scanf("%i", &id);
                if(getCustomerById(id) != NULL) {
                    removeRecord(db, id);
                } else {
                    printf("\n-----------------------------------\n"
                           "Картка з таким номером(ID) не знайдена!\n"
                           "-----------------------------------\n");
                }
                break;
            }
            case 6: {
                redactRecords(db);
                break;
            }
            case 7: {
                uploadClientsBase(db);
                break;
            }
            case 8: {
                sqlite3_close(db);
                free(customers);
                free(events);
                return 0;
            }
            default: {
                printf("\n-----------------------------\n"
                       "Введено недопустиме значення!\n"
                       "-----------------------------\n");
            }
        }
    }
    return 0;
}







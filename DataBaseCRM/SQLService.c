//
//  SQLService.c
//  DataBaseCRM
//
//  Created by Svitlana Korostelova on 23.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#include "SQLService.h"

void readCustomers(sqlite3 *db) {
    resetCustomers();
    int rc;
    char *sql = calloc(100,sizeof(char));
    sql = "SELECT * FROM CUSTOMERS ORDER BY title;";
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, sql, readCustomersCallback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

int readCustomersCallback(void *NotUsed, int argc, char **argv, char **azColName){
    struct Customer * temp = (struct Customer *)malloc(sizeof(struct Customer));
    // Основываясь на том, что строго знаем порядок колонок в БД SQL и их имена:
    
    temp->id = atoi(argv[0]); // приводим к int, т.к. в SQL всё - символы
    temp->title = (char *)malloc(strlen(argv[1])); // Измеряем содержимое строки в колонке argv[1], чтоб выделить динамически память и копируем в temp
    strcpy(temp->title, argv[1]);
    temp->contactPerson = (char *)malloc(strlen(argv[2]));
    strcpy(temp->contactPerson, argv[2]);
    temp->contactInfo = (char *)malloc(strlen(argv[3]));
    strcpy(temp->contactInfo, argv[3]);
    
    customers[customersCount] = *temp;
    customersCount++;
    // Записываем в массив структур готового клиента из структуры temp, сдвигаем счётчик по массиву структур и на каждом новом витке обнуляем temp (в месте вызова данная функция будет циклично запущена столько раз, сколько строк в SQL для каждой)
    free(temp);
    return 0;
}

int readEventsCallback(void *NotUsed, int argc, char **argv, char **azColName){
    struct Event * temp = (struct Event *)malloc(sizeof(struct Event));
    // Сравниваем или название пришедшей в аргументе azColName i-той колонки равно заданой, тогда берем i-тое значение из argv
    for(int i = 0; i < argc; i++) {
        if(strcmp(azColName[i], "id") == 0) {
            temp->id = atoi(argv[i]); // Приводим к int, т.к. в SQL всё - символы
        } else if(strcmp(azColName[i], "customerId") == 0) {
            int customerId = atoi(argv[i]);
            temp->customer = getCustomerById(customerId);
        } else if(strcmp(azColName[i], "eventType") == 0) {
            temp->eventType = atoi(argv[i]);
        } else if(strcmp(azColName[i], "description") == 0) {
            temp->description = (char *)malloc(100);
            strcpy(temp->description, argv[i]);
        }
    }
    events[eventsCount] = *temp;
    eventsCount++;
    // Записываем в массив структур готовое событие из структуры temp, сдвигаем счётчик по массиву структур и на каждом новом витке обнуляем temp (в месте вызова данная функция будет циклично запущена столько раз, сколько строк в SQL для каждой)
    free(temp);
    return 0;
}

void resetEvents(void) {
    events = (struct Event *)malloc(400*sizeof(struct Event));
    eventsCount = 0;
}
void resetCustomers(void) {
    customers = (struct Customer *)malloc(200*sizeof(struct Customer));
    customersCount = 0;
}

struct Customer * getCustomerById(int ID) {
    for(int i = 0; i < customersCount; i++) {
        if(customers[i].id == ID) {
            return &customers[i];
        }
    }
    return NULL;
}

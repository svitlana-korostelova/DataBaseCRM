//
//  addEvent.c
//  DataBaseCRM
//
//  Created by Svitlana Korostelova on 16.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#include "SQLService.h"

void addNewEvent(sqlite3 *db) {
    readCustomers(db);
    printf("Введіть номер(ID) картки клієнта, за якою відбулись нові події:\n");
    int customerId;
    scanf("%i",&customerId);
    if(getCustomerById(customerId) == NULL) {
        printf("\n-----------------------------------\n"
               "Картка з таким номером(ID) не знайдена!\n"
               "-----------------------------------\n");
        return;
    }
    int eventType = 0;

    do {
        printf("\n=========================================================\n"
               "Введіть подію цифрою за зразком:\n"
               "1 - Подзвонити\n"
               "2 - Підписати договір\n"
               "3 - Виставити акт виконаних робіт\n"
               "4 - Прийняти платіж\n"
               "5 - Домовленості з відстрочки платежу чи термінів виконання\n"
               "===========================================================\n");
        scanf("%i", &eventType);
        fflush(stdin);
    } while (eventType<1||eventType>5);
    
    char *sql = calloc(1000,sizeof(char));
    char *description = calloc(100,sizeof(char));
    printf("Заповніть дату події, опис, примітки: \n");
    scanf("\n");
    scanf("%[^\n]", description);
    description = realloc(description, strlen(description) + 1);
    
    sprintf(sql, "INSERT INTO EVENTS(customerId, eventType, description) VALUES ('%i','%i','%s');",customerId, eventType, description);
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
        printf("\nЗміни внесено успішно!\n");
}


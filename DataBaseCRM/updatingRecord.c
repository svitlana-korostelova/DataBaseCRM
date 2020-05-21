//
//  updatingRecord.c
//  DataBaseCRM
//
//  Created by Svitlana Korostelova on 14.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#include "SQLService.h"

void redactRecords(sqlite3 *db) {
     readCustomers(db);
    printf("Введіть номер(ID) картки клієнта, що потребує змін:\n");
    int id;
    scanf("%i",&id);
    if(getCustomerById(id) == NULL) {
        printf("\n-----------------------------------\n"
               "Картка з таким номером(ID) не знайдена!\n"
               "-----------------------------------\n");
        return;
}
    
    int redactMenu = 0;
    do {
        printf("\n===================================================\n"
               "Виберіть одну з наступних цифр для редагування поля:\n"
               "1 - Назва організації\n"
               "2 - Контактна особа\n"
               "3 - Контактні дані\n"
               "===================================================\n");
        scanf("%i", &redactMenu);
        fflush(stdin);
    } while (redactMenu<1||redactMenu>3);

    
    switch(redactMenu){
        case 1: {
            // для поля title
            char *title = calloc(100,sizeof(char));
            printf("Внесіть зміни в назву організації:\n");
            scanf("\n");
            scanf("%[^\n]", title);
            updatingRecord(db,title,id,"title");
            free(title);
            break;}
        case 2: {
            // для поля contactPerson
            char *contactPerson = calloc(100,sizeof(char));
            printf("Внесіть зміни в дані про контактну особу:\n");
            scanf("\n");
            scanf("%[^\n]", contactPerson);
            updatingRecord(db,contactPerson,id,"contactPerson");
            free(contactPerson);
            break;}
        case 3: {
            // для поля contactInfo
            char *contactInfo = calloc(300,sizeof(char));
            printf("Внесіть зміни в контактну інформацію:\n");
            scanf("\n");
            scanf("%[^\n]", contactInfo);
            updatingRecord(db,contactInfo,id,"contactInfo");
            free(contactInfo);
            break;}
    }
}

void updatingRecord(sqlite3 *db, char *selectedData, int id, char *columnName) {
    char *sql = calloc(1000,sizeof(char));

sprintf(sql, "UPDATE CUSTOMERS SET %s= '%s' WHERE id= '%i';", columnName,selectedData, id);

    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
        printf("\nЗміни внесено успішно!\n");
}

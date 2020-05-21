//
//  downloadClientsBase.c
//  DataBaseCRM
//
//  Created by Svitlana Korostelova on 16.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#include "SQLService.h"

void downloadClientsBase(sqlite3 *db){
    char *str = calloc(300,sizeof(char));
    
    FILE *fin;
    char fileName[50];
    printf("Введіть ім'я файлу для завантаження даних з розширенням включно:\n");
    scanf("%s", fileName);
    fflush(stdin);
    fin=fopen(fileName,"r"); //"dataBase.txt" в папке проекта
 
    if (fin==NULL){
        fprintf(stderr, "Error opening file. Error code: %d\n", errno);
        return;
    }

    while(!feof(fin)) {
        char *title = calloc(100,sizeof(char));
        char *contactPerson = calloc(100,sizeof(char));
        char *contactInfo = calloc(100,sizeof(char));
        
        fscanf(fin, "\n");
        fscanf(fin, "%[^\n]", str);
        // title
        int nTitle = strstr(str, "|") - str; // нашли символ АСКИ где указатель на | и отняли от него символ АСКИ куда указывает str (в данном случае на начале строки стоит)
        memcpy(title, str, nTitle);// копируем куда, откуда, сколько символов
        str = str + nTitle + 1;// сместить указатель в текущей строке на уже прочитанное слово и символ |
        
        // contactPerson
        int nContactPerson = strstr(str, "|") - str;
        memcpy(contactPerson, str, nContactPerson);
        str = str + nContactPerson + 1;
        
        // contactInfo
        int nContactInfo = strstr(str, "|") - str;
        memcpy(contactInfo, str, nContactInfo);
        str = str + nTitle + 1;
        
        insertCustomerToSQL(title, contactPerson, contactInfo, db);
    }
    fclose(fin);
}

void insertCustomerToSQL(char *title, char *contactPerson, char *contactInfo, sqlite3 *db) {
    char *sql = calloc(300,sizeof(char));
    sprintf(sql, "INSERT INTO CUSTOMERS(title, contactPerson, contactInfo) VALUES ('%s', '%s', '%s');", title, contactPerson, contactInfo);
    char *zErrMsg;
    int rc;
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
    free(zErrMsg);
}

void accessLoadDataFromFile(sqlite3 *db) {
    printf("Для завантаження даних з файлів потрібні права адміністратора!!!\n");
    int password;
    int i=0;
    for (; i<3; i++) {
        printf("Введіть пароль:\n");
        scanf("%i",&password);
        fflush(stdin);
        if (password==1111) {
            downloadClientsBase(db);
            printf("\nДані в базу додано успішно!\n");
            break;
        }
    }
    if (i >=3) {
        printf("Спроби вичерпано. Повернення до головного меню:\n");
    }
}

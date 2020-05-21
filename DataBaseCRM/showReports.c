//
//  showReports.c
//  DataBaseCRM
//
//  Created by Svitlana Korostelova on 19.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#include "SQLService.h"

int showReports(sqlite3 *db) {
    readCustomers(db);
    printf("\n=========================================\n"
           "Введіть один з наступних підпунктів меню:\n"
           "1 - Загальна відомість по клієнтам\n"
           "2 - Звіт подій по клієнту\n"
           "3 - Вибірка клієнтів за статусом взаємин\n"
           "4 - Пошук інформації за частиною слова\n"
           "5 - ВИЙТИ В ОСНОВНЕ МЕНЮ\n"
           "=========================================\n");
    int subparagraph;
    scanf("%i", &subparagraph);
    switch(subparagraph){
        case 1: {
            printf("\t\t\t\t\tЗАГАЛЬНА ВІДОМІСТЬ ПО КЛІЄНТАХ\n");
printf("--------------------------------------------------------------------------------------------\n");
            printf("|№ID |  Назва організації  |     Контактна особа           |     Контактна інформація      |\n");
printf("--------------------------------------------------------------------------------------------\n");
            for (int i=0; i<customersCount; i++) {
                printf("| %-3i| %-20s| %-30s| %-30s|\n", customers[i].id, customers[i].title, customers[i].contactPerson, customers[i].contactInfo);
            }
printf("--------------------------------------------------------------------------------------------\n");
            break;
        }
        case 2: {
            int customerId;
            //№ID - номер карточки клиента, он уникален в рамках БД и автоинкрементируется в таблице SQL, по этому ID осуществляется учёт связаной таблицы БД Events. Пользователь базы или обычно знает ID клиента или может вывести общий отчёт или отчёт по части слова и посмотреть каой у клиента ID.
            printf("Введіть номер(ID) картки клієнта:\n");
            scanf("%i", &customerId);
            
            if(getCustomerById(customerId) == NULL) {
                        printf("\n-----------------------------------\n"
                       "Картка з таким номером(ID) не знайдена!\n"
                       "-----------------------------------\n");
                break;
            }
            readEventsForOneCustomer(db,customerId);
            printf("\t\t\t\t\tЗВІТ ПОДІЙ ПО КЛІЄНТУ № %i\n",customerId);
printf("--------------------------------------------------------------------------------------------------------------\n");
            printf("| Тип події  |  Назва організації  |     Контактна особа           |         Опис стану взаємин              |\n");
printf("--------------------------------------------------------------------------------------------------------------\n");
            for (int i=0; i<eventsCount; i++) {
                int eventType = events[i].eventType; //взять из enum число и как итый символ прочитать его текстовое значение в массиве
  
                printf("|%-12s| %-20s| %-30s| %-40s|\n", EVENT_TYPE_STRING[eventType], events[i].customer->title,events[i].customer->contactPerson, events[i].description);
            }
printf("--------------------------------------------------------------------------------------------------------------\n");
            break;
        }
            
        case 3:{
            int eventType = 0;
            do {
                printf("\n================================\n"
                       "Введіть подію цифрою за зразком:\n"
                       "1 - Подзвонити\n"
                       "2 - Підписати договір\n"
                       "3 - Виставити акт виконаних робіт\n"
                       "4 - Прийняти платіж\n"
                       "5 - Домовленості з відстрочки платежу чи термінів виконання\n"
                       "================================\n");
                scanf("%i", &eventType);
                fflush(stdin);
            } while (eventType<1||eventType>5);
            readCustomersForOneEvent(db,eventType);
            printf("\n");
            printf("\t\t\tПЕРЕЛІК КЛІЄНТІВ ЗА ОБРАНИМ ЕТАПОМ РОБОТИ З НИМИ\n");
printf("--------------------------------------------------------------------------------------------------------------\n");
            printf("|  Тип події  |  Назва організації  |     Контактна особа           |         Опис стану взаємин              |\n");
printf("--------------------------------------------------------------------------------------------------------------\n");
            for (int i=0; i<eventsCount; i++) {
                int eventType = events[i].eventType;
                
                printf("| %-12s| %-20s| %-30s| %-40s|\n", EVENT_TYPE_STRING[eventType], events[i].customer->title,events[i].customer->contactPerson, events[i].description);
            }
printf("--------------------------------------------------------------------------------------------------------------\n");
            break;
        }
        case 4: {
            selectingByPartOfWord(db);
 printf("--------------------------------------------------------------------------------------------\n");
             printf("|№ID |  Назва організації  |     Контактна особа           |     Контактна інформація      |\n");
 printf("--------------------------------------------------------------------------------------------\n");
             for (int i=0; i<customersCount; i++) {
                 printf("| %-3i| %-20s| %-30s| %-30s|\n", customers[i].id, customers[i].title, customers[i].contactPerson, customers[i].contactInfo);
             }
 printf("--------------------------------------------------------------------------------------------\n");
            break;
        }
        case 5: {
            return 0;
        }
        default: {
            printf("\n-----------------------------\n"
                   "Введено недопустиме значення!\n"
                   "-----------------------------\n");
            return 1;
        }
    }
    
    return 0;
}


void readEventsForOneCustomer(sqlite3 *db, int customerId) {
    readCustomers(db);
    resetEvents();
    int rc;
    char *sql = calloc(100,sizeof(char));
    sprintf(sql, "SELECT * FROM EVENTS WHERE customerId = %i ORDER BY eventType;", customerId);
    // Дальше sqlite3_exec читает все данные согласно запросу sql и возвращает каждую строку таблицы (каждую запись) в readEventsCallback - аналог цикла для каждой записи
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, sql, readEventsCallback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

void readCustomersForOneEvent(sqlite3 *db, EventType eventType) {
    readCustomers(db); // Перечитать всех клиентов в массив структур, если там сейчас отфильтрованы только некоторые из них
    resetEvents();
    int rc;
    char *sql = calloc(100,sizeof(char));
    sprintf(sql, "SELECT * FROM EVENTS WHERE eventType = %i;", eventType);
    char *zErrMsg = 0;
    rc = sqlite3_exec(db, sql, readEventsCallback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

void selectingByPartOfWord(sqlite3 *db) {
    resetCustomers();
    char *sql = calloc(1000,sizeof(char));
    char *partOfWord = calloc(100,sizeof(char));
    printf("Введiть iнформацiю для пошуку:\n");
    scanf("\n");
    scanf("%[^\n]", partOfWord);
    partOfWord = realloc(partOfWord, strlen(partOfWord) + 1);
    printf("\tРезультати пошуку по |%s|:\n",partOfWord);
    
    sprintf(sql, "SELECT * FROM customers "
            "where title  like '%%%s%%'"
            "or contactPerson  like '%%%s%%'"
            "or contactInfo like '%%%s%%'",partOfWord,partOfWord,partOfWord);
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db, sql, readCustomersCallback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}





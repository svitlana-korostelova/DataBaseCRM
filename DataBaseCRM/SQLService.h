//
//  SQLService.h
//  DataBaseCRM
//
//  Created by Svitlana Korostelova on 11.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#ifndef SQLService_h
#define SQLService_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sqlite3.h>

typedef enum {
    call = 1,
    deal = 2,
    docAct = 3,
    payment = 4,
    delay = 5
} EventType;

static const char *EVENT_TYPE_STRING[] = {
    "", "Call", "Deal", "Act of works", "Payment", "Delay docs",
};

struct Customer{
    int id;
    char *title;
    char *contactPerson;
    char *contactInfo;
};

struct Event {
    int id;
    EventType eventType;
    char * description;
    struct Customer * customer;
};

extern int customersCount;
extern struct Customer *customers;

extern int eventsCount;
extern struct Event *events;

void accessLoadDataFromFile(sqlite3 *db);
/* Проверяет право на загрузку информации пакетом в базу, для этого нужно знать пароль администратора, если валидный - вызывает downloadClientsBase */

void downloadClientsBase(sqlite3 *db);
/* Принимает от пользователя имя файла, откуда будет загружена информация построчно в цикле с помощью адресной арифметики указателей и также построчно вызывает insertCustomerToSQL для записи данных в БД */

void addCustomer(sqlite3 *db);
/* Принимает от пользователя реквизиты нового клиента (записывает в БД, вызывая insertCustomerToSQL) */

void removeRecord(sqlite3 *db, int id);
/* Удаляет из БД SQL запись по клиенту. Каскадно удаляет по нём записи в таблице Events, связанной по уникальному ID */

void redactRecords(sqlite3 *db);
/* Даёт пользователю выбрать поле, которое нужно изменить в БД и отдаёт в updatingRecord */

void updatingRecord(sqlite3 *db, char *selectedData, int id, char *columnName);
/* Записывает из redactRecords в SQL изменение по соответствию поля, принятого от пользователя с полем таблицы БД */

void addNewEvent(sqlite3 *db);
/* Даёт пользователю возможность выбрать из перечисления предопределённые события, ввести описание к событию и записывает строку в БД SQL в таблицу Events, связавшись по уникальному ID клиента */

void uploadClientsBase(sqlite3 *db);
/* Выгружает в файл содержание таблицы сustomers, разделяя поля знаком "|" */

void insertCustomerToSQL(char *title, char *contactPerson, char *contactInfo, sqlite3 *db);
/* Записывает параметры нового клиента в БД по соответствию полей, принятых от пользователя с полями таблицы БД. Также записывает в цикле пакетом клиентов при первичной загрузке */

void readCustomers(sqlite3 *db);
/* Актуализирует из БД SQL полную структуру клиентов в место вызова (в том числе вызывает resetCustomers - перезагрузку массива структур Customer, выделяя под него динамически память для новой работы). readCustomers нужен, когда после предыдущих действий пользователя (например отбор по части слова) в массиве структур Customer храниться неполная информация и её нужно актуализировать. Вызывает циклично для каждой строки из БД SQL readCustomersCallback */

int readCustomersCallback(void *NotUsed, int argc, char **argv, char **azColName);
// Обходит циклично для каждой строки из БД SQL (таблица customers) и читает для текущей в argc - количество колонок в БД, argv - столько же значений сколько колонок для текущей строки в таблице БД, azColName - массивы символов, состоящие из имён колонок БД

int readEventsCallback(void *NotUsed, int argc, char **argv, char **azColName);
// Обходит циклично для каждой строки из БД SQL (таблица events) и читает для текущей: argc - количество колонок, argv - значения в строке для каждой колонки, azColName - названия самих колонок

void resetEvents(void);
/* Выделяет динамически память на массив структур Events, обнуляет счётчик по нему */

void resetCustomers(void);
/* Обновляет массив структур Customer, выделяя под него динамически память для новой работы и обнуляет счётчик по массиву */

struct Customer * getCustomerById(int ID);
/* Возвращает элемент struct Customer по его id. Если клиента нет в БД - вернёт в место вызова NULL, затем пользователю будет выдано в месте вызова сообщение об отсутствии */

int showReports(sqlite3 *db);
/* 4 ключевых отчёты БД */
void readCustomersForOneEvent(sqlite3 *db, EventType eventType);
/* Показывает отчёт по всем клиентам для одного заданого пользователем события */
void readEventsForOneCustomer(sqlite3 *db, int customerId);
/* Показывает отчёт по всем событиям для одного клиента */
void selectingByPartOfWord(sqlite3 *db);
/* Показывает отчёт по любому введённому пользователем значению (например часть имени или телефона). Ищет введённый набор символов в БД SQL (таблица customers) и выводит все строки, где есть поля с соответствием */

#endif /* SQLService_h */



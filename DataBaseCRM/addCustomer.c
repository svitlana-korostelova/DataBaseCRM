//
//  addCustomer.c
//  DataBaseCRM
//
//  Created by Svitlana Korostelova on 13.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#include "SQLService.h"

void addCustomer(sqlite3 *db) {
    char *title = calloc(100,sizeof(char));
    printf("Введiть назву організації:\n");
    
    //    gets(title);
    scanf("\n");
    scanf("%[^\n]", title);
    title = realloc(title, strlen(title) + 1);
    
    
    char *contactPerson = calloc(100,sizeof(char));
    printf("Введіть контактну особу:\n");
    
    //    gets(contactPerson);
    scanf("\n");
    scanf("%[^\n]", contactPerson);
    contactPerson = realloc(contactPerson, strlen(contactPerson) + 1);
    
    char *contactInfo = calloc(100,sizeof(char));
    printf("Введіть телефон, адресу, примітки, тощо:\n");
    
    //    gets(contactInfo);
    scanf("\n");
    scanf("%[^\n]", contactInfo);
    contactInfo = realloc(contactInfo, strlen(contactInfo) + 1);
    
   insertCustomerToSQL(title, contactPerson, contactInfo, db);
    printf("\nДані в базу додано успішно!\n");
}

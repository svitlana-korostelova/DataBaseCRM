//
//  uploadClientsBase.c
//  DataBaseCRM
//
//  Created by Svitlana Korostelova on 19.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#include "SQLService.h"

void uploadClientsBase(sqlite3 *db){
    readCustomers(db);
    FILE *fout = fopen("exportDB.txt","w+");
    
    for (int i=0; i<customersCount; i++) {
        fprintf(fout,"%s|", customers[i].title);
        fprintf(fout,"%s|", customers[i].contactPerson);
        fprintf(fout,"%s|\n", customers[i].contactInfo);
    }
    fclose (fout);
    printf("\nДані вивантажено успішно!\n");
}



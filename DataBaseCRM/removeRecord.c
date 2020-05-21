//
//  removeRecord.c
//  testSqlite
//
//  Created by Svitlana Korostelova on 11.03.2020.
//  Copyright © 2020 Svitlana Korostelova. All rights reserved.
//

#include "SQLService.h"

void removeRecord(sqlite3 *db, int id) {
    readCustomers(db);
    char *sql = calloc(100,sizeof(char));
    sprintf(sql, "DELETE FROM Customers WHERE id = %i;"
                "DELETE FROM Events WHERE customerId = %i;", id, id);
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    printf("\nВидалено успішно!\n");
}

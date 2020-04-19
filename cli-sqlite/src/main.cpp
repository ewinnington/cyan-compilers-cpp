// main.cpp
#include <sqlite3.h>
#include <stdio.h>

//default callback function showing results of statement executions
//
//typedef int (*sqlite3_callback)(
//   void*,    /* Data provided in the 4th argument of sqlite3_exec() */
//   int,      /* The number of columns in row */
//   char**,   /* An array of strings representing fields in the row */
//   char**    /* An array of strings representing column names */
//);
//
static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    if (data != NULL)
        fprintf(stderr, "%s: ", (const char *)data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main()
{
    printf("SQLITE3 Library version: %s\n", sqlite3_libversion());

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    rc = sqlite3_open(":memory:", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create TABLE SQL statement */
    sql = "CREATE TABLE COMPANY("
          "ID INT PRIMARY KEY     NOT NULL,"
          "NAME           TEXT    NOT NULL,"
          "AGE            INT     NOT NULL,"
          "ADDRESS        CHAR(50),"
          "SALARY         REAL );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }

    /* Create INSERT SQL statement */
    sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
          "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
          "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
          "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
          "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

    /* Execute INSERT statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Records created successfully\n");
    }

    /* SQL WITH BINDS INTO A prepared statement */
    sqlite3_stmt *stmt;
    sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
          "VALUES (?, ?, ?, ?, ? );";

    if ( sqlite3_prepare( db, sql, -1, &stmt, 0) != SQLITE_OK ) 
    {
        printf("\nCould not prepare statement. %s", sql);
    }
    sqlite3_bind_int(stmt, 1, 5); 
    sqlite3_bind_text(stmt, 2, "Eric", 4, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, 40); 
    sqlite3_bind_text(stmt, 4, "Zurich", 6, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 5, 12345.6);
    //sqlite3_bind_null() - unused here 
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
    printf("\nCould not step (execute) stmt.\n");
    }
    //before re-executing the statement, you need to reset the statement 
    sqlite3_reset(stmt);

    /* SELECT statement */
    sql = "SELECT * from COMPANY";

    const char *data = "Record output:";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }

    sqlite3_close(db);
    return 0;
}
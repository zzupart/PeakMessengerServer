#pragma once

#include "server.h"

int dbConnect(std::string arg_name, sqlite3* arg_db) {
    int rc = sqlite3_open(arg_name.c_str(), &arg_db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(arg_db) << std::endl;
        return rc;
    }
    else {
        std::cout << "Database opened successfully." << std::endl;
        return SQLITE_OK;
    }
    
}

int dbInit(sqlite3* arg_db) { 
    std::cerr << "cringe";
    return SQLITE_OK;
}
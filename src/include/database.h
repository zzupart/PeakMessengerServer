#pragma once

#include "server.h"

int dbConnect(std::string arg_name, sqlite3** arg_db) {
    int rc = sqlite3_open(arg_name.c_str(), arg_db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(*arg_db) << std::endl;
        return rc;
    }
    std::clog << "Database opened successfully." << std::endl;
    return SQLITE_OK;
}

int dbInit(sqlite3* arg_db) { 
    char* requestSQL = "PRAGMA foreign_keys = ON;";
    int rc = sqlite3_exec(arg_db, requestSQL, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(arg_db) << std::endl;
        return rc;
    }
    std::clog << "Pragma 1 executed successfully\n";

    requestSQL = "CREATE TABLE IF NOT EXISTS users ("
       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
       "login VARCHAR NOT NULL,"
       "password VARCHAR NOT NULL);";
    rc = sqlite3_exec(arg_db, requestSQL, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(arg_db) << std::endl;
        return rc;
    }
    std::clog << "Table 'users' created successfully\n";

    requestSQL = "CREATE TABLE IF NOT EXISTS chats ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT);";
    rc = sqlite3_exec(arg_db, requestSQL, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(arg_db) << std::endl;
        return rc;
    }
    std::clog << "Table 'chats' created successfully\n";

    requestSQL = "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "msg_text VARCHAR NOT NULL,"
        "user_id INTEGER NOT NULL,"
        "chat_id INTEGER NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (chat_id) REFERENCES chats(id));";
    rc = sqlite3_exec(arg_db, requestSQL, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(arg_db) << std::endl;
        return rc;
    }
    std::clog << "Table 'messages' created successfully\n";

    requestSQL = "CREATE TABLE IF NOT EXISTS userchat ("
        "user_id INTEGER NOT NULL,"
        "chat_id INTEGER NOT NULL,"
        "PRIMARY KEY (user_id, chat_id),"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (chat_id) REFERENCES chats(id));";
    rc = sqlite3_exec(arg_db, requestSQL, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(arg_db) << std::endl;
        return rc;
    }
    std::clog << "Table 'userchat' created successfully\n";

    return rc;
}
#pragma once

#include "server.h"

sqlite3* db = nullptr;
std::unordered_map<std::string, sqlite3_stmt*> stmtMap;

int DBConnect(std::string arg_name) {
    int rc = sqlite3_open(arg_name.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        exit(rc);
    }
    std::clog << "Database opened successfully." << std::endl;
    return SQLITE_OK;
}

int DBInit() { 
    char* query = "PRAGMA foreign_keys = ON;";
    int rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl;
        exit(rc);
    }
    std::clog << "Pragma 1 executed successfully\n";

    query = "CREATE TABLE IF NOT EXISTS users ("
       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
       "login VARCHAR(20) NOT NULL,"
       "password VARCHAR NOT NULL);";
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl;
        exit(rc);
    }
    std::clog << "Table 'users' created successfully\n";

    query = "CREATE TABLE IF NOT EXISTS chats ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT);";
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl;
        exit(rc);
    }
    std::clog << "Table 'chats' created successfully\n";

    query = "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "msg_text VARCHAR(512) NOT NULL,"
        "user_id INTEGER NOT NULL,"
        "chat_id INTEGER NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (chat_id) REFERENCES chats(id));";
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl;
        exit(rc);
    }
    std::clog << "Table 'messages' created successfully\n";

    query = "CREATE TABLE IF NOT EXISTS userchat ("
        "user_id INTEGER NOT NULL,"
        "chat_id INTEGER NOT NULL,"
        "PRIMARY KEY (user_id, chat_id),"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (chat_id) REFERENCES chats(id));";
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl;
        exit(rc);
    }
    std::clog << "Table 'userchat' created successfully\n";

    return rc;
}

int PrepareStatements() {
    char* query = "SELECT * FROM users WHERE login = ?";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmtMap["CheckIfLoginIsUsed"], nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        exit(rc);
    }
 
    query = "INSERT INTO users (login, password) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, query, -1, &stmtMap["RegisterUser"], nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        exit(rc);
    }

    return rc;
}

void DBClear() {
    sqlite3_close(db);
    sqlite3_finalize(stmtMap["CheckIfLoginIsUsed"]);
    sqlite3_finalize(stmtMap["RegisterUser"]);
}
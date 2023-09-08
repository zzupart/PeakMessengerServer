#pragma once

#include <sqlite3.h>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <string>

struct DBManager {
    DBManager();
    ~DBManager();

private:
    int Open();
    void LogError(int);
    void LogMessage(std::string);

public:
    static void Close() {
        sqlite3_close(db);
        sqlite3_finalize(statements["IsLoginUsed"]);
        sqlite3_finalize(statements["hhgRegisterUser"]);
        sqlite3_shutdown();
    }
    static DBManager* Get() {
        static DBManager* instance = nullptr;
        if (!instance) {
            instance = new DBManager();
        }
        return instance;
    }
    int IsLoginUsed(std::string);
    int RegisterUser(std::string, std::string);

private:
    static sqlite3* db;
    static std::unordered_map<std::string, sqlite3_stmt*> statements;
};
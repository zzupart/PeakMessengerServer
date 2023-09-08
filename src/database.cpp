#include "database.h"

sqlite3* DBManager::db = nullptr;
std::unordered_map<std::string, sqlite3_stmt*> DBManager::statements;

DBManager::DBManager() {
	int rc = this->Open();
	if (rc != SQLITE_OK) {
		this->LogError(rc);
		exit(1);
	}
}

DBManager::~DBManager() {
    Close();
}

int DBManager::Open() {
    sqlite3_initialize();
	int rc = sqlite3_open("userinfo.db", &db);
	if (rc != SQLITE_OK) {
		this->LogError(rc);
		exit(1);
	}
	this->LogMessage("Database opened successfully");

    char* query = "PRAGMA foreign_keys = ON;";
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        this->LogError(rc);
        exit(rc);
    }
    this->LogMessage("Pragma 1 executed successfully");

    query = "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "login VARCHAR(20) NOT NULL,"
        "password VARCHAR(64) NOT NULL);";
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        this->LogError(rc);
        exit(rc);
    }
    this->LogMessage("Table 'users' created successfully");

    query = "CREATE TABLE IF NOT EXISTS chats ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT);";
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        this->LogError(rc); 
        exit(rc); 
    }
    this->LogMessage("Table 'chats' created successfully");

    query = "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "msg_text VARCHAR(512) NOT NULL,"
        "user_id INTEGER NOT NULL,"
        "chat_id INTEGER NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (chat_id) REFERENCES chats(id));";
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        this->LogError(rc);
        exit(rc);
    }
    this->LogMessage("Table 'messages' created successfully");

    query = "CREATE TABLE IF NOT EXISTS userchat ("
        "user_id INTEGER NOT NULL,"
        "chat_id INTEGER NOT NULL,"
        "PRIMARY KEY (user_id, chat_id),"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (chat_id) REFERENCES chats(id));";
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        this->LogError(rc);
        exit(rc);
    }
    this->LogMessage("Table 'userchat' created successfully");
     
    query = "SELECT * FROM users WHERE login = ?";
    rc = sqlite3_prepare_v2(db, query, -1, &statements["IsLoginUsed"], nullptr);
    if (rc != SQLITE_OK) {
        this->LogError(rc);
        exit(rc);
    }

    query = "INSERT INTO users (login, password) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, query, -1, &statements["RegisterUser"], nullptr);
    if (rc != SQLITE_OK) {
        this->LogError(rc);
        exit(rc);
    }

    return rc;
}

void DBManager::LogError(int arg_rc) {
	spdlog::error("An error occured when working with a db: '" + std::string(sqlite3_errmsg(db)) + "' with the error code " + std::to_string(arg_rc));
}

void DBManager::LogMessage(std::string arg_message) { 
	spdlog::info(arg_message);
}

int DBManager::IsLoginUsed(std::string arg_login) {
    int rc = sqlite3_bind_text(statements["IsLoginUsed"], 1, arg_login.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        this->LogError(rc);
        return 1;
    }

    rc = sqlite3_step(statements["IsLoginUsed"]);
    sqlite3_reset(statements["IsLoginUsed"]);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        this->LogError(rc);
    }

    return rc;
}

int DBManager::RegisterUser(std::string arg_login, std::string arg_passwordHash) {
    int rc = sqlite3_bind_text(statements["RegisterUser"], 1, arg_login.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_reset(statements["RegisterUser"]);
        this->LogError(rc);
        return rc;
    }
    rc = sqlite3_bind_text(statements["RegisterUser"], 2, arg_passwordHash.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_reset(statements["RegisterUser"]);
        this->LogError(rc);
        return rc;
    }

    rc = sqlite3_step(statements["RegisterUser"]);
    sqlite3_reset(statements["RegisterUser"]);
    if (rc != SQLITE_DONE) {
        this->LogError(rc);
    }

    return rc;
}
#pragma once

#include "database.h"
#include "server.h"
#include "sha256.h"

crow::response HandleHomeRoute(const crow::request& arg_req) {
	return crow::response(200, "OK");
}

crow::response HandleRegister(const crow::request& arg_req) {
    if (arg_req.get_header_value("Content-Type") == "application/x-www-form-urlencoded") {
        if (!arg_req.url_params.get("login")) {
            return crow::response(400, "Missing 'login' parameter");
        }
        if (!arg_req.url_params.get("password")) {
            return crow::response(400, "Missing 'password' parameter");
        }
        std::string login = arg_req.url_params.get("login");
        std::string password = arg_req.url_params.get("password");
        if (password.size() < 8) {
            return crow::response(400, "Password is too short");
        }

        int rc = sqlite3_bind_text(stmtMap["CheckIfLoginIsUsed"], 1, login.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            std::cerr << "There was an error binding 'CheckIfLoginUsed' statement" << sqlite3_errmsg(db) << std::endl;
            return crow::response(500, "An internal server error occured");
        }

        rc = sqlite3_step(stmtMap["CheckIfLoginIsUsed"]);
        if (rc == SQLITE_ROW) {  
            sqlite3_reset(stmtMap["CheckIfLoginIsUsed"]);
            return crow::response(409, "Login is already taken");
        }
        else if (rc == SQLITE_DONE) { }
        else {
            sqlite3_reset(stmtMap["CheckIfLoginIsUsed"]);
            std::cerr << "There was an error executing 'CheckIfLoginUsed' statement\n";
            return crow::response(500, "An internal server error occured");
        }
        sqlite3_reset(stmtMap["CheckIfLoginIsUsed"]);

        std::string passwordHash = sha256(password);

        rc = sqlite3_bind_text(stmtMap["RegisterUser"], 1, login.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            sqlite3_reset(stmtMap["RegisterUser"]);
            std::cerr << "There was an error binding 'RegisterUser' statement\n";
            return crow::response(500, "An internal server error occured");
        }
        rc = sqlite3_bind_text(stmtMap["RegisterUser"], 2, passwordHash.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) { 
            sqlite3_reset(stmtMap["RegisterUser"]);
            std::cerr << "There was an error binding 'RegisterUser' statement\n";
            return crow::response(500, "An internal server error occured"); 
        }

        rc = sqlite3_step(stmtMap["RegisterUser"]);
        if (rc == SQLITE_DONE) {
            sqlite3_reset(stmtMap["RegisterUser"]);
            return crow::response(200, "User added successfully");
        }
        else {
            std::cerr << "There was an error executing 'RegisterUser' statement" << sqlite3_errmsg(db) << std::endl;
            sqlite3_reset(stmtMap["RegisterUser"]);
            return crow::response(500, "An internal server error occured");
        }
    }
    else {
        return crow::response(400, "Invalid Content-Type. Only form data is accepted.");
    }
}
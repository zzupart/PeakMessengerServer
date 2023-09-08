#pragma once

#include "sha256.h"
#include "database.h"
#include "common.h"
#include <unordered_map>
#include <string>

crow::SimpleApp app;

namespace Routes {

crow::response HandleHomeRoute(const crow::request& arg_req) {
    return crow::response(200, "OK");
}

crow::response HandleRegisterRoute(const crow::request& arg_req) {
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

    int rc = DBManager::Get()->IsLoginUsed(login);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        return crow::response(500, "An internal server error occured");
    }
    else if (rc == SQLITE_DONE) {
        return crow::response(409, "Login is already taken");
    }

    std::string passwordHash = sha256(password);
    rc = DBManager::Get()->RegisterUser(login, passwordHash);
    if (rc != SQLITE_DONE) {
        return crow::response(500, "An internal server error occured");
    }

    return crow::response(200, "User successfully added");
}

}
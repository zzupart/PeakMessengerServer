#pragma once

#include "server.h"

crow::response handleHomeRoute(const crow::request& arg_req) {
	return crow::response(200, "OK");
}

crow::response handleRegister(const crow::request& arg_req) {
    if (arg_req.get_header_value("Content-Type") == "application/x-www-form-urlencoded") {
        if (!arg_req.url_params.get("login")) {
            return crow::response(400, "Missing 'login' parameter");
        }
        if (!arg_req.url_params.get("password")) {
            return crow::response(400, "Missing 'password' parameter");
        }
        std::string login = arg_req.url_params.get("login");
        std::string password = arg_req.url_params.get("password");

        // check if user is not in the db
        return crow::response(409, "Login already taken");

        // add user to the db

        // Return a success message
        return crow::response(200, "Account registered successfully!");
    }
    else {
        return crow::response(400, "Invalid Content-Type. Only form data is accepted.");
    }
}
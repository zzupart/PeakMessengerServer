#include "routes.h"
#include "server.h"
#include "database.h"

int main() {
	/*if (sqlite3_initialize() != SQLITE_OK) {
		std::cerr << "SQLite initialization failed." << std::endl;
		return 1;
	}
	sqlite3* db;
	int rc = dbConnect("userinfo.db", db);
	if (rc != SQLITE_OK) { 
		return 1;
	}
	dbInit(db);
	sqlite3_close(db);

	CROW_ROUTE(app, "/").methods("GET"_method)(handleHomeRoute);
	CROW_ROUTE(app, "/register").methods("POST"_method)(handleRegister);

	app.port(2323).multithreaded().run();

	return 0;*/
}
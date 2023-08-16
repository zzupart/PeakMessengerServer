#include "routes.h"
#include "server.h"
#include "database.h"

int main() {
	int rc = sqlite3_initialize();
	if (rc != SQLITE_OK) {
		std::cerr << "SQLite initialization failed." << std::endl;
		return rc;
	}
	sqlite3* db = nullptr;
	rc = dbConnect("userinfo.db", &db);
	if (rc != SQLITE_OK) { 
		sqlite3_close(db);
		return rc;
	}
	rc = dbInit(db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return rc;
	}
	sqlite3_close(db);

	CROW_ROUTE(app, "/").methods("GET"_method)(handleHomeRoute);
	CROW_ROUTE(app, "/register").methods("POST"_method)(handleRegister);

	app.port(2323).multithreaded().run();

	return 0;
}
#include "routes.h"
#include "server.h"
#include "database.h"

int main() {
	int rc = sqlite3_initialize();
	if (rc != SQLITE_OK) {
		std::cerr << "SQLite initialization failed." << std::endl;
		exit(rc);
	}
	DBConnect("userinfo.db");
	DBInit();
	PrepareStatements();

	CROW_ROUTE(app, "/").methods("GET"_method)(HandleHomeRoute);
	CROW_ROUTE(app, "/register").methods("POST"_method)(HandleRegister);

	std::atexit(DBClear);
	app.port(2323).multithreaded().run();

	return 0;
}
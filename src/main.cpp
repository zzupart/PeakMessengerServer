#include "routes.h" 

int main() {
   	DBManager::Get(); 

	CROW_ROUTE(app, "/").methods("GET"_method)(Routes::HandleHomeRoute); 
	CROW_ROUTE(app, "/register").methods("POST"_method)(Routes::HandleRegisterRoute);

	std::atexit(DBManager::Get()->Close);
	app.port(2323).multithreaded().run(); 

	return 0;
}
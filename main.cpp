#include <pqxx/pqxx>
#include <iostream>
#include <tuple>
#include <Windows.h>

#include <iostream>

#pragma execution_character_set("utf-8");

class transaction_base {

   private:
	  void exec(std::string requestText) { // универсальный метод запросов
		pqxx::connection c(
			"host=localhost "
			"port=5432 "
			"dbname=customerManagement "
			"user=postgres password=123");
		pqxx::work tx{ c };
		tx.exec(requestText);
		tx.commit();
      }
	  void query(std::string requestText) {
		  pqxx::connection c(
			  "host=localhost "
			  "port=5432 "
			  "dbname=customerManagement "
			  "user=postgres password=123");
		  pqxx::work tx{ c };
		  for (auto [customer_id, firstName, lastName, email, nomerFone] : tx.query<int, std::string, std::string, std::string, std::string>(
			  requestText))
		  {
			  std::cout << customer_id << "  " << firstName << "  " << lastName << "  " << email << "  " << nomerFone << ".\n";
		  }

	  }


   public:
	  void createTable(std::string name_table) { // создание таблицы клиентов с уникальным названием и связанной с ней таблицей телефонов клиента
		  transaction_base::exec("CREATE TABLE IF NOT EXISTS "  + name_table + "("
			  "customer_id SERIAL PRIMARY KEY, "
			  "firstName VARCHAR(40) NOT NULL, "
			  "lastName VARCHAR(40) NOT NULL, "
			  "email VARCHAR(40) NOT NULL); "
			  "CREATE TABLE IF NOT EXISTS "  + name_table + "fone ("
			  "fone_id SERIAL PRIMARY KEY, "
			  "nomerFone VARCHAR(40) unique, "
		      "customer_id INTEGER  REFERENCES " + name_table + "(customer_id));");
	  }

	  void addCustomer(std::string name_table, int customer_id, std::string firstName, std::string lastName, std::string email) { // добавление клиента в таблицу
		  transaction_base::exec("INSERT INTO " + name_table +
			  " VALUES(" + pqxx::to_string(customer_id) + ", " +
			  firstName+ ", " +
			  lastName + ", " +
			  email + ");");
	  }
	  void addPhoneCustomer(std::string name_table, int fone_id, std::string nomerFone, int customer_id) { // добавление телефона клиента в таблицу телефонов
		  transaction_base::exec("INSERT INTO " + name_table + "fone " +
			  "VALUES(" + pqxx::to_string(fone_id) + ", " +
			  nomerFone + ", " +
			  pqxx::to_string(customer_id) + ");");
	  }
	  void updateCustomer(std::string name_table, int customer_id, std::string firstName, std::string lastName, std::string email) { // изменение данных клиента в таблицу
		  transaction_base::exec("UPDATE " + name_table +
			  " SET firstName="+firstName + 
			  " ,lastName="+lastName  +
			  " ,email="+email  +
			  " WHERE customer_id=" + pqxx::to_string(customer_id)+ ";");
	  }
	  void deletePhoneCustomer(std::string name_table, int customer_id) { // удаление телефонов клиента в таблицe телефонов
		  transaction_base::exec("DELETE FROM " + name_table + "fone " +
			  " WHERE customer_id=" + pqxx::to_string(customer_id) + ";");
	  }
	  void deleteCustomer(std::string name_table, int customer_id) { // удаление тклиента в таблицe
		  transaction_base::exec("DELETE FROM " + name_table  +
			  " WHERE customer_id=" + pqxx::to_string(customer_id) + ";");
	  }
	  void selectCustomer(std::string name_table, std::string firstName, std::string lastName, std::string email, std::string nomerFone) { // добавление клиента в таблицу
		     transaction_base::query("SELECT " + name_table + ".customer_id, firstName, lastName, email, " + name_table + "fone.nomerFone" +
			  " FROM " + name_table +
			  " JOIN " + name_table + "fone " +
			  " ON " + name_table + ".customer_id=" +
			  name_table + "fone.customer_id " +
			  " WHERE firstName=" + firstName + " OR lastName=" + lastName + " OR email=" + email + " OR " + name_table + "fone.nomerFone=" + nomerFone);
	  }
};

  
int main(){ 
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
   try{	
	  transaction_base  x; // создание объекта класса transaction_base 
	  x.createTable("customer2"); // создание таблицы клиентов с уникальным названием 
	  x.addCustomer("customer2", 4, "'Pet'", "'Ivan'", "'Ivan@com.com'"); // добавление нового клиента
	  x.addPhoneCustomer("customer2", 2, "'8999999998'", 3);// добавление телефона клиента
	  x.updateCustomer("customer2", 1, "'Sergey'", "'Sidorov'", "'Sidorov@com.com'");//изменение данных клиента
	  x.deletePhoneCustomer("customer2", 1);// удаление телефона клиента в таблицe
	  x.deleteCustomer("customer2", 1);// удаление клиента
	  x.selectCustomer("customer2", "'n'", "'n'", "'n'", "'7999999998'"); //поиск в таблице по телефону, неизвестные параметры указываются как n
	  x.selectCustomer("customer2", "'Petr'", "'Ivan'", "'n'", "'n'"); // поиск в таблице по имени и фамилии

   } catch (const std::exception& e){
	    std::cout << e.what() << std::endl;
   }
  return EXIT_SUCCESS;
}
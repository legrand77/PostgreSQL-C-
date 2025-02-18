#include <pqxx/pqxx>
#include <iostream>
#include <tuple>
#include <Windows.h>

#include <iostream>

#pragma execution_character_set("utf-8");

class transaction_base {

   private:
	  void exec(std::string requestText) { // ������������� ����� ��������
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
	  void createTable(std::string name_table) { // �������� ������� �������� � ���������� ��������� � ��������� � ��� �������� ��������� �������
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

	  void addCustomer(std::string name_table, int customer_id, std::string firstName, std::string lastName, std::string email) { // ���������� ������� � �������
		  transaction_base::exec("INSERT INTO " + name_table +
			  " VALUES(" + pqxx::to_string(customer_id) + ", " +
			  firstName+ ", " +
			  lastName + ", " +
			  email + ");");
	  }
	  void addPhoneCustomer(std::string name_table, int fone_id, std::string nomerFone, int customer_id) { // ���������� �������� ������� � ������� ���������
		  transaction_base::exec("INSERT INTO " + name_table + "fone " +
			  "VALUES(" + pqxx::to_string(fone_id) + ", " +
			  nomerFone + ", " +
			  pqxx::to_string(customer_id) + ");");
	  }
	  void updateCustomer(std::string name_table, int customer_id, std::string firstName, std::string lastName, std::string email) { // ��������� ������ ������� � �������
		  transaction_base::exec("UPDATE " + name_table +
			  " SET firstName="+firstName + 
			  " ,lastName="+lastName  +
			  " ,email="+email  +
			  " WHERE customer_id=" + pqxx::to_string(customer_id)+ ";");
	  }
	  void deletePhoneCustomer(std::string name_table, int customer_id) { // �������� ��������� ������� � ������e ���������
		  transaction_base::exec("DELETE FROM " + name_table + "fone " +
			  " WHERE customer_id=" + pqxx::to_string(customer_id) + ";");
	  }
	  void deleteCustomer(std::string name_table, int customer_id) { // �������� �������� � ������e
		  transaction_base::exec("DELETE FROM " + name_table  +
			  " WHERE customer_id=" + pqxx::to_string(customer_id) + ";");
	  }
	  void selectCustomer(std::string name_table, std::string firstName, std::string lastName, std::string email, std::string nomerFone) { // ���������� ������� � �������
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
	  transaction_base  x; // �������� ������� ������ transaction_base 
	  x.createTable("customer2"); // �������� ������� �������� � ���������� ��������� 
	  x.addCustomer("customer2", 4, "'Pet'", "'Ivan'", "'Ivan@com.com'"); // ���������� ������ �������
	  x.addPhoneCustomer("customer2", 2, "'8999999998'", 3);// ���������� �������� �������
	  x.updateCustomer("customer2", 1, "'Sergey'", "'Sidorov'", "'Sidorov@com.com'");//��������� ������ �������
	  x.deletePhoneCustomer("customer2", 1);// �������� �������� ������� � ������e
	  x.deleteCustomer("customer2", 1);// �������� �������
	  x.selectCustomer("customer2", "'n'", "'n'", "'n'", "'7999999998'"); //����� � ������� �� ��������, ����������� ��������� ����������� ��� n
	  x.selectCustomer("customer2", "'Petr'", "'Ivan'", "'n'", "'n'"); // ����� � ������� �� ����� � �������

   } catch (const std::exception& e){
	    std::cout << e.what() << std::endl;
   }
  return EXIT_SUCCESS;
}
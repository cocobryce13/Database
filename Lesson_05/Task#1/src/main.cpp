#include <iostream>
#include <pqxx/pqxx>

class ClientManager {
public:
    ClientManager(const std::string& connStr) : connection(connStr) {}

    void createTables() {
        pqxx::work txn(connection);
        txn.exec("CREATE TABLE IF NOT EXISTS Client (id SERIAL PRIMARY KEY, first_name VARCHAR(100), last_name VARCHAR(100), email VARCHAR(100) UNIQUE);");
        txn.exec("CREATE TABLE IF NOT EXISTS Phone (id SERIAL PRIMARY KEY, client_id INTEGER REFERENCES Client(id) ON DELETE CASCADE, phone_number VARCHAR(20));");
        txn.commit();
    }

    void addClient(std::string first, std::string last, std::string email) {
        pqxx::work txn(connection);
        txn.exec("INSERT INTO Client (first_name, last_name, email) VALUES ('" + first + "', '" + last + "', '" + email + "')");
        txn.commit();
    }

    void addPhone(int clientId, std::string phone) {
        pqxx::work txn(connection);
        txn.exec("INSERT INTO Phone (client_id, phone_number) VALUES (" + std::to_string(clientId) + ", '" + phone + "')");
        txn.commit();
    }

    void updateClient(int clientId, std::string first, std::string last, std::string email) {
        pqxx::work txn(connection);
        txn.exec("UPDATE Client SET first_name = '" + first + "', last_name = '" + last + "', email = '" + email + "' WHERE id = " + std::to_string(clientId));
        txn.commit();
    }

    void deletePhone(int phoneId) {
        pqxx::work txn(connection);
        txn.exec("DELETE FROM Phone WHERE id = " + std::to_string(phoneId));
        txn.commit();
    }

    void deleteClient(int clientId) {
        pqxx::work txn(connection);
        txn.exec("DELETE FROM Client WHERE id = " + std::to_string(clientId));
        txn.commit();
    }

    void findClient(std::string keyword) {
        pqxx::work txn(connection);
        std::string query = "SELECT c.id, c.first_name, c.last_name, c.email, p.phone_number FROM Client c LEFT JOIN Phone p ON c.id = p.client_id WHERE c.first_name ILIKE '%" + keyword + "%' OR c.last_name ILIKE '%" + keyword + "%' OR c.email ILIKE '%" + keyword + "%' OR p.phone_number ILIKE '%" + keyword + "%'";
        pqxx::result r = txn.query(query);

        for (auto row : r) {
            std::cout << "Client ID: " << row["id"].as<int>() << ", Name: " << row["first_name"].c_str() << " " << row["last_name"].c_str() << ", Email: " << row["email"].c_str() << ", Phone: ";
            if (row["phone_number"].is_null()) std::cout << "N/A" << std::endl;
            else std::cout << row["phone_number"].c_str() << std::endl;
        }
    }

private:
    pqxx::connection connection;
};

int main() {
    try {
        pqxx::connection conn(
            "host=localhost "
            "port=5432 "
            "dbname=my_database "
            "user=my_database_user "
            "password=my_password_123"
        );

        ClientManager cm("host=localhost port=5432 dbname=my_database user=my_database_user password=my_password_123");

        cm.createTables();
        cm.addClient("Ivan", "Ivanov", "ivan@example.com");
        cm.addPhone(1, "+79161234567");
        cm.updateClient(1, "Ivan", "Petrov", "ivanpetrov@example.com");
        cm.findClient("Ivan");
        cm.deletePhone(1);
        cm.deleteClient(1);

    } catch (pqxx::sql_error e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
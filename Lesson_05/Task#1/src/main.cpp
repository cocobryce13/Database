#include <iostream>
#include <pqxx/pqxx>
#include <vector>
#include <tuple>

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
        txn.exec("INSERT INTO Client (first_name, last_name, email) VALUES ('" + txn.esc(first) + "', '" + txn.esc(last) + "', '" + txn.esc(email) + "')");
        txn.commit();
    }

    void addPhone(int clientId, std::string phone) {
        pqxx::work txn(connection);
        txn.exec("INSERT INTO Phone (client_id, phone_number) VALUES (" + txn.esc(std::to_string(clientId)) + ", '" + txn.esc(phone) + "')");
        txn.commit();
    }

    void updateClient(int clientId, std::string first, std::string last, std::string email) {
        pqxx::work txn(connection);
        txn.exec("UPDATE Client SET first_name = '" + txn.esc(first) + "', last_name = '" + txn.esc(last) + "', email = '" + txn.esc(email) + "' WHERE id = " + txn.esc(std::to_string(clientId)));
        txn.commit();
    }

    void deletePhone(int phoneId) {
        pqxx::work txn(connection);
        txn.exec("DELETE FROM Phone WHERE id = " + txn.esc(std::to_string(phoneId)));
        txn.commit();
    }

    void deleteClient(int clientId) {
        pqxx::work txn(connection);
        txn.exec("DELETE FROM Client WHERE id = " + txn.esc(std::to_string(clientId)));
        txn.commit();
    }

    std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> findClient(std::string keyword) {
        pqxx::work txn(connection);
        std::string query = "SELECT c.id, c.first_name, c.last_name, c.email, p.phone_number FROM Client c LEFT JOIN Phone p ON c.id = p.client_id WHERE c.first_name ILIKE '%" + txn.esc(keyword) + "%' OR c.last_name ILIKE '%" + txn.esc(keyword) + "%' OR c.email ILIKE '%" + txn.esc(keyword) + "%' OR p.phone_number ILIKE '%" + txn.esc(keyword) + "%'";
        pqxx::result r = txn.query(query);

        std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> results;
        for (auto row : r) {
            results.push_back({
                row["id"].as<int>(),
                row["first_name"].c_str(),
                row["last_name"].c_str(),
                row["email"].c_str(),
                row["phone_number"].is_null() ? "" : row["phone_number"].c_str()
            });
        }
        return results;
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

        auto clients = cm.findClient("Ivan");
        for (auto& c : clients) {
            std::cout << "Client ID: " << std::get<0>(c) << ", "
                      << std::get<1>(c) << " " << std::get<2>(c) << ", "
                      << std::get<3>(c) << ", Phone: " << std::get<4>(c) << std::endl;
        }

        cm.deletePhone(1);
        cm.deleteClient(1);

    } catch (pqxx::sql_error e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
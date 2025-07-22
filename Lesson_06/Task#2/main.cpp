#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <iostream>
#include <string>
#include <memory>

namespace dbo = Wt::Dbo;

class Publisher;
class Book;
class Shop;

class Publisher
{
public:
    std::string name;

    dbo::collection<dbo::ptr<Book>> books;

    template<class Action>
    void persist(Action& a)
{
        dbo::field(a, name, "name");
        dbo::hasMany(a, books, dbo::ManyToOne, "publisher");
    }
};

class Book
{
public:
    std::string title;
    dbo::ptr<Publisher> publisher;

    template<class Action>
    void persist(Action& a)
{
        dbo::field(a, title, "title");
        dbo::belongsTo(a, publisher, "publisher");
    }
};

class Shop
{
public:
    std::string name;
    dbo::collection<dbo::ptr<Book>> books;

    template<class Action>
    void persist(Action& a)
{
        dbo::field(a, name, "name");
        dbo::hasMany(a, books, dbo::ManyToMany, "shop_book");
    }
};

int main()
{
    try
{
        std::string connStr = "host=localhost port=5432 dbname=bookdb user=bookuser password=bookpass";
        auto postgres = std::make_unique<dbo::backend::Postgres>(connStr);
        dbo::Session session;
        session.setConnection(std::move(postgres));

        session.mapClass<Publisher>("publisher");
        session.mapClass<Book>("book");
        session.mapClass<Shop>("shop");

        try
{
            session.createTables();
        } catch (...)
{}
{
            dbo::Transaction t(session);

            auto pub = session.add(std::make_unique<Publisher>());
            pub.modify()->name = "Огни";

            auto book1 = session.add(std::make_unique<Book>());
            book1.modify()->title = "Ветры";
            book1.modify()->publisher = pub;

            auto book2 = session.add(std::make_unique<Book>());
            book2.modify()->title = "Пески";
            book2.modify()->publisher = pub;

            auto shop = session.add(std::make_unique<Shop>());
            shop.modify()->name = "Букмаркет";
            shop.modify()->books.insert(book1);
            shop.modify()->books.insert(book2);

            t.commit();
        }

        std::string publisherName;
        std::cout << "Введите имя издателя: ";
        std::getline(std::cin, publisherName);

        dbo::Transaction t(session);
        auto pub = session.find<Publisher>().where("name = ?").bind(publisherName);

        if (!pub.empty())
{
            for (auto& p : pub)
{
                std::cout << "Издатель: " << p->name << std::endl;
                for (const auto& book : p->books)
{
                    std::cout << "  Книга: " << book->title << std::endl;
                    for (const auto& shop : session.find<Shop>())
{
                        for (const auto& b : shop->books)
{
                            if (b.id() == book.id())
{
                                std::cout << "    Магазин: " << shop->name << std::endl;
                            }
                        }
                    }
                }
            }
        } else
{
            std::cout << "Издатель не найден.
";
        }
        t.commit();

    } catch (const dbo::Exception& e)
{
        std::cout << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
#include <Wt/Dbo/Dbo.h>

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
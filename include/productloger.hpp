#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>
#include <string>
#include <vector>

using namespace eosio;
using std::string;
using std::vector;

CONTRACT productloger : public contract
{
public:
    using contract::contract;
    productloger(name receiver, name code, datastream<const char *> ds)
        : contract(receiver, code, ds),
          users(receiver, receiver.value),
          products(receiver, receiver.value),
          tags(receiver, receiver.value),
          ids(receiver, receiver.value) {}

    ACTION adduser(name user,
                   name manager,
                   string description);
    ACTION addmanager(name manager,
                      string description);
    ACTION rmuser(name user);
    ACTION rmmanager(name manager);

    ACTION logproduct(uint64_t product_tag,
                     string product_name,
                     string description,
                     name logger);
    ACTION logowner(uint64_t product_id,
                    name current_owner,
                    name new_owner);                 
    ACTION logrecord(uint64_t product_id, 
                    string description, 
                    name logger);
    ACTION setproducttag(uint64_t product_id,
                        uint64_t new_product_tag,
                        name logger);
    ACTION sold(uint64_t product_id,
                    name logger);

    uint64_t get_next_id();

private:
    struct owner
    {
        name logger;
        name owner;
        time_point_sec date_logged;
    };
    struct record
    {
        name logger;
        string description;
        time_point_sec date_logged;
    };

    struct status
    {
        name logger;
        bool sold = false;
        time_point_sec date_logged;
    };

    TABLE product
    {
        uint64_t id;
        uint64_t tag;
        name creator;
        string productname;
        string description;
        time_point_sec created;
        vector<owner> owners;
        vector<record> records;
        status sold;
        uint64_t primary_key() const { return id; }
        uint64_t by_tag() const { return tag; }
    };
    typedef multi_index<"product"_n, product,
                        indexed_by<"bytag"_n, const_mem_fun<product, uint64_t, &product::by_tag>>>
        product_index;
    product_index products;

    TABLE user
    {
        name id;
        name manager;
        string description;
        uint64_t primary_key() const { return id.value; }
    };
    typedef multi_index<"user"_n, user> user_index;
    user_index users;

    TABLE tag
    {
        uint64_t id;
        uint64_t product_id;
        uint64_t primary_key() const { return id; }
    };
    typedef multi_index<"tag"_n, tag> tag_index;
    tag_index tags;

    struct id
    {
        uint64_t value;
    };
    typedef singleton<"id"_n, id> id_index;
    id_index ids;
};
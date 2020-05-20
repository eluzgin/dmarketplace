#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/time.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>
#include <string>
#include <vector>

using namespace eosio;
using std::string;
using std::vector;

CONTRACT dmarketplace : public contract
{
public:
    using contract::contract;
    dmarketplace(name receiver, name code, datastream<const char *> ds)
        : contract(receiver, code, ds),
          listings(receiver, receiver.value),
          products(receiver, receiver.value),
          categories(receiver, receiver.value),
          selloptions(receiver, receiver.value),
          orders(receiver, receiver.value),
          disputes(receiver, receiver.value),
          reviews(receiver, receiver.value),
          reputations(receiver, receiver.value),
          bans(receiver, receiver.value),
          ids(receiver, receiver.value) {}

    ACTION addproduct(string product_name,
                     string description,
                     double price,
                     string currency,
                     string thumbnail,
                     string image,
                     uint64_t category_id,
                     int stock,
                     name author);

    ACTION updproduct(uint64_t id,
                     string product_name,
                     string description,
                     double price,
                     string currency,
                     string thumbnail,
                     string image,
                     uint64_t category_id,
                     int stock,
                     name author);

    ACTION addcategory(string category_name,
                       string description,
                       string thumbnail,
                       name author); 

    ACTION updcategory(uint64_t id,
                       string category_name,
                       string description,
                       string thumbnail,
                       name author);               

    ACTION addpayaddr(string currency, 
                    string address,
                    name seller);

    ACTION createorder(uint64_t product_id, 
                    int quantity,
                    double total, 
                    string currency,
                    name buyer); 

    ACTION marksold(uint64_t product_id,
                    string paytrx,
                    name author);

    ACTION addreview(int rank,
                    string comments,
                    name actor,
                    name reviewer);

    ACTION adddispute(uint64_t order_id,
                    name seller,
                    name buyer,
                    string complaint,
                    name author);

    ACTION appenddispute(uint64_t id, 
                    string type,
                    string description,
                    name author);
    
    ACTION banactor(name actor,
        uint64_t dispute_id,
        string comments);

    [[eosio::on_notify("eosio.token::transfer")]]
    void ontransfer(name from, name to, asset quantity, string memo);                

    uint64_t get_next_id();

private:
    struct record
    {
        name author;
        string type;
        string description;
        time_point_sec created;
    };

    struct payoption
    {
        string currency;
        string payaddress;
    };

    struct id
    {
        uint64_t value;
    };
    typedef singleton<"id"_n, id> id_index;
    id_index ids;

    TABLE listing 
    {
        uint64_t id;
        uint64_t product_id;
        double listing_fee;
        name seller;
        time_point_sec created;
        uint64_t primary_key() const { return id; }
    };
    typedef multi_index<"listing"_n, listing> listing_index;
    listing_index listings;

    TABLE selloption
    {
        name seller;
        vector<payoption> payoptions;
        uint64_t primary_key() const { return seller.value;}
    };
    typedef multi_index<"selloption"_n, selloption> selloption_index;
    selloption_index selloptions;

    TABLE product
    {
        uint64_t id;
        name seller;
        string product_name;
        string description;
        double price;
        string currency;
        string thumbnail;
        string image;
        uint64_t category_id;
        int stock;
        int status;
        time_point_sec created;
        uint64_t primary_key() const { return id; }
    };
    typedef multi_index<"product"_n, product> product_index;
    product_index products;

    TABLE category
    {
        uint64_t id;
        name creator;
        string category_name;
        string description;
        string thumbnail;
        time_point_sec created;
        uint64_t primary_key() const { return id; }
    };
    typedef multi_index<"category"_n, category> category_index;
    category_index categories;

    TABLE order
    {
        uint64_t id;
        uint64_t product_id;
        name buyer;
        name seller;
        int quantity;
        double total;
        string currency;
        string payaddress;
        string paystatus;
        string paytrx;
        int status;
        uint64_t dispute_id;
        time_point_sec created;
        uint64_t primary_key() const { return id; }
    };
    typedef multi_index<"order"_n, order> order_index;
    order_index orders;

    TABLE dispute
    {
        uint64_t id;
        uint64_t order_id;
        name seller;
        name buyer;
        vector<record> records;
        int status;
        time_point_sec created;
        time_point_sec updated;
        uint64_t primary_key() const { return id; }
    };
    typedef multi_index<"dispute"_n, dispute> dispute_index;
    dispute_index disputes;

    TABLE review
    {
        uint64_t id;
        name actor;
        name reviewer;
        int rank;
        string comments;
        time_point_sec created;
        uint64_t primary_key() const { return id; }
    };
    typedef multi_index<"review"_n, review> review_index;
    review_index reviews;

    TABLE reputation
    {
        uint64_t id;
        name actor;
        int deals;
        int avg_rank;
        time_point_sec updated;
        uint64_t primary_key() const { return id; }
    };
    typedef multi_index<"reputation"_n, reputation> reputation_index;
    reputation_index reputations;

    TABLE ban
    {
        uint64_t id;
        name actor;
        uint64_t dispute_id;
        string comments;
        time_point_sec created;
        uint64_t primary_key() const { return id; }
    };
    typedef multi_index<"ban"_n, ban> ban_index;
    ban_index bans;

};

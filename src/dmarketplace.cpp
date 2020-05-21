#include <dmarketplace.hpp>

uint64_t dmarketplace::get_next_id()
{
    id next_id{ids.exists() ? ids.get().value + 1 : 1};
    ids.set(next_id, get_self());
    return next_id.value;
}

void dmarketplace::addproduct(string product_name,
                     string description,
                     double price,
                     string currency,
                     string thumbnail,
                     string image,
                     uint64_t category_id,
                     int stock,
                     name author)
{
    require_auth(author);
    check(!product_name.empty(), "product name cannot be empty");
    check(!description.empty(), "product description cannot be empty");
    check(!currency.empty(), "selected currency cannot be empty");
    check(!thumbnail.empty(), "thumbnail url cannot be empty");
    check(!image.empty(), "image url cannot be empty");
    products.emplace(get_self(), [&](auto &row) {
        row.id = get_next_id();
        row.seller = author;
        row.product_name = product_name;
        row.description = description;
        row.price = price;
        row.currency = currency;
        row.thumbnail = thumbnail;
        row.image = image;
        row.category_id = category_id;
        row.stock = stock;
        row.created = time_point_sec(current_time_point());
        row.status = 0;
        row.created = time_point_sec(current_time_point());
    });

}

void dmarketplace::updproduct(uint64_t id,
                     string product_name,
                     string description,
                     double price,
                     string currency,
                     string thumbnail,
                     string image,
                     uint64_t category_id,
                     int stock,
                     name author) 
{
    require_auth(author);
    auto product_itr = products.find(id);
    check(product_itr != products.end(), "that product id does not exist");
    string error_msg = author.to_string() + " is not authorized to execute this action";
    check(product_itr->seller == author, error_msg);
    check(!product_name.empty(), "product name cannot be empty");
    check(!description.empty(), "product description cannot be empty");
    check(!currency.empty(), "selected currency cannot be empty");
    check(!thumbnail.empty(), "thumbnail url cannot be empty");
    check(!image.empty(), "image url cannot be empty");
    products.modify(product_itr, get_self(), [&](auto &row) {
        row.product_name = product_name;
        row.description = description;
        row.price = price;
        row.currency = currency;
        row.thumbnail = thumbnail;
        row.image = image;
        row.category_id = category_id;
        row.stock = stock;
    });
}

void dmarketplace::addcategory(string category_name,
                       string description,
                       string thumbnail,
                       name author) 
{
    require_auth(author);
    check(!category_name.empty(), "category name cannot be empty");
    check(!description.empty(), "description cannot be empty");
    check(!thumbnail.empty(), "thumbnail url cannot be empty");
    categories.emplace(get_self(), [&](auto &row) {
        row.id = get_next_id();
        row.creator = author;
        row.category_name = category_name;
        row.description = description;
        row.thumbnail = thumbnail;
        row.created = time_point_sec(current_time_point());
    });
}

void dmarketplace::updcategory(uint64_t id,
                       string category_name,
                       string description,
                       string thumbnail,
                       name author)
{
    require_auth(author);
    auto cat_itr = categories.find(id);
    check(cat_itr != categories.end(), "that category id does not exist");
    string error_msg = author.to_string() + " is not authorized to execute this action";
    check(cat_itr->creator == author, error_msg);
    check(!category_name.empty(), "category name cannot be empty");
    check(!description.empty(), "description cannot be empty");
    check(!thumbnail.empty(), "thumbnail url cannot be empty");
    categories.modify(cat_itr, get_self(), [&](auto &row) {
        row.creator = author;
        row.category_name = category_name;
        row.description = description;
        row.thumbnail = thumbnail;
    });
}                       

void dmarketplace::addpayaddr(string currency, 
                    string address,
                    name seller)
{
    require_auth(seller);
    check(!currency.empty(), "currency cannot be empty");
    check(!address.empty(), "payment address cannot be empty");
    auto iterator = selloptions.find(seller.value);
    payoption opt = {currency, address};
    if( iterator == selloptions.end() )
    {
        selloptions.emplace(get_self(), [&]( auto& row ) {
            row.seller = seller;
            row.payoptions.push_back(opt);
        });
    }
    else {
        selloptions.modify(iterator, get_self(), [&](auto &row) {
            row.payoptions.push_back(opt);
        });
    }
}

void dmarketplace::createorder(uint64_t product_id, 
                    int quantity,
                    double total, 
                    string currency,
                    name buyer)
{
    require_auth(buyer);
    auto product_itr = products.find(product_id);
    check(product_itr != products.end(), "that product id does not exist");
    name seller;
    products.modify(product_itr, get_self(), [&](auto &row) {
        row.stock = row.stock - quantity;
        seller = row.seller;
    });
    auto selloptions_itr = selloptions.find(seller.value);
    check(selloptions_itr != selloptions.end(), "seller is not registered");
    vector<payoption> payoptions = selloptions_itr->payoptions;
    string seller_pay_address;
    for( const auto& option: payoptions ) {
        if( option.currency == currency ) {
            seller_pay_address = option.payaddress;
        }
    }
    check(!seller_pay_address.empty(), "seller payment address is not found for currency " + currency);
    orders.emplace(get_self(), [&](auto &row) {
        row.id = get_next_id();
        row.product_id = product_id;
        row.buyer = buyer;
        row.seller = seller;
        row.quantity = quantity;
        row.total = total;
        row.currency = currency;
        row.payaddress = seller_pay_address;
        row.paystatus = "UNPAID";
        row.status = 0; // pending
        row.created = time_point_sec(current_time_point());
    });
}


void dmarketplace::marksold(uint64_t order_id,
                    string payment_trx,
                    name seller) 
{
    require_auth(seller);
    auto order_itr = orders.find(order_id);
    check(order_itr != orders.end(), "that order id does not exist");
    check(order_itr->seller == seller, "only seller can mark order as sold");
    orders.modify(order_itr, get_self(), [&](auto &row) {
        row.status = 1; // sold
        row.paystatus = "PAID"; 
        row.paytrx = payment_trx;
    });
}

void dmarketplace::addreview(int rank,
                    string comments,
                    name actor,
                    name reviewer) 
{
    require_auth(reviewer);
    check(!comments.empty(), "comments cannot be empty");
    reviews.emplace(get_self(), [&](auto &row) {
        row.id = get_next_id();
        row.actor = actor;
        row.reviewer = reviewer;
        row.rank = rank;
        row.comments = comments;
        row.created = time_point_sec(current_time_point());
    });
}

void dmarketplace::adddispute(uint64_t order_id,
                    name seller,
                    name buyer,
                    string complaint,
                    name author) 
{
    require_auth(author);
    check(!complaint.empty(), "complaint description cannot be empty");
    check((author == buyer || author == seller), "Only buyer or seller can file a complaint");
    auto order_itr = orders.find(order_id);
    check(order_itr != orders.end(), "that order id does not exist");
    check(order_itr->seller == seller, "seller does not match seller in order");
    check(order_itr->buyer == buyer, "buyer does not match buyer in order");
    record rec = {author, "CREATED", complaint, time_point_sec(current_time_point())};
    disputes.emplace(get_self(), [&](auto &row) {
        row.id = get_next_id();
        row.order_id = order_id;
        row.seller = seller;
        row.buyer = buyer;
        row.status = 0; // created
        row.records.push_back(rec);
        row.created = time_point_sec(current_time_point());
    });
}

void dmarketplace::appenddispute(uint64_t id, 
                    string type,
                    string description,
                    name author)
{
    require_auth(author);
    check(!type.empty(), "type cannot be empty");
    check(!description.empty(), "description cannot be empty");
    auto disp_itr = disputes.find(id);
    check(disp_itr != disputes.end(), "that dispute id does not exist");
    record rec = {author, type, description, time_point_sec(current_time_point())};
    disputes.modify(disp_itr, get_self(), [&](auto &row) {
        row.status = 1;
        row.updated = time_point_sec(current_time_point());
        row.records.push_back(rec);
    });
}
    
void dmarketplace::banactor(name actor,
        uint64_t dispute_id,
        string comments)
{
    check(get_self().to_string == "dmarketplace", "Unauthorized to ban user");
    bans.emplace(get_self(), [&](auto &row) {
        row.id = get_next_id();
        row.actor = actor;
        row.dispute_id = dispute_id;
        row.comments = comments;
        row.created = time_point_sec(current_time_point());
    });
}

[[eosio::on_notify("eosio.token::transfer")]]
void dmarketplace::ontransfer(name from, name to, asset quantity, string memo)
{
    if(from.to_string == "dmarketplace") {
        return;
    }
    check(quantity.amount > 0, "Must be positive deposit amount");
    check(!memo.empty(), "memo must contain product id");

    int prod_id = std::stoi(memo);
    listings.emplace(get_self(), [&](auto &row) {
        row.id = get_next_id();
        row.product_id = prod_id;
        row.seller = from;
        row.listing_fee = quantity.amount;
        row.created = time_point_sec(current_time_point());
    });
}







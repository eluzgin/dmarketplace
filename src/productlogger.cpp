#include <productlogger.hpp>

uint64_t productlogger::get_next_id()
{
    id next_id{ids.exists() ? ids.get().value + 1 : 1};
    ids.set(next_id, get_self());
    return next_id.value;
}

void productlogger::adduser(name user, name manager, string description)
{
    require_auth(manager);
    check(user != manager, "user and manager cannot be equal");

    string error_msg = user.to_string() + " does not exist on chain";
    check(is_account(user), error_msg.c_str());

    auto manager_itr = users.find(manager.value);
    error_msg = manager.to_string() + " is not a user";
    check(manager_itr != users.end(), error_msg.c_str());

    error_msg = manager.to_string() + " is not a manager";
    check(manager_itr->manager == manager, error_msg.c_str());

    auto user_itr = users.find(user.value);
    error_msg = user.to_string() + " is already a user";
    check(user_itr == users.end(), error_msg.c_str());

    users.emplace(get_self(), [&](auto &row) {
        row.id = user;
        row.manager = manager;
        row.description = description;
    });
}

void productlogger::rmuser(name user)
{
    auto user_itr = users.find(user.value);
    string error_msg = user.to_string() + " is not a user";
    check(user_itr != users.end(), error_msg.c_str());
    error_msg = user.to_string() + " is a manager";
    check(user != user_itr->manager, error_msg.c_str());
    require_auth(user_itr->manager);
    users.erase(user_itr);
}

void productlogger::addmanager(name manager, string description)
{
    require_auth(get_self());

    string error_msg = manager.to_string() + " does not exist on chain";
    check(is_account(manager), error_msg.c_str());

    auto user_itr = users.find(manager.value);
    error_msg = manager.to_string() + " is already a manager";
    check(user_itr == users.end(), error_msg.c_str());

    users.emplace(get_self(), [&](auto &row) {
        row.id = manager;
        row.manager = manager;
        row.description = description;
    });
}

void productlogger::rmmanager(name manager)
{
    require_auth(get_self());
    auto user_itr = users.find(manager.value);
    string error_msg = manager.to_string() + " is not a user";
    check(user_itr != users.end(), error_msg.c_str());
    error_msg = manager.to_string() + " is not a manager";
    check(user_itr->manager == manager, error_msg.c_str());
    users.erase(user_itr);
}

void productlogger::logproduct(uint64_t product_tag,
                             string product_name,
                             string product_description,
                             name logger)
{
    require_auth(logger);
    auto user_itr = users.find(logger.value);
    string error_msg = logger.to_string() + " is not a user";
    check(user_itr != users.end(), error_msg.c_str());
    auto tag_itr = tags.find(product_tag);
    check(tag_itr == tags.end(), "that product tag already exists");
    check(!product_breed.empty(), "product breed cannot be empty");
    check(!product_origin.empty(), "product origin cannot be empty");
    check(!product_location.empty(), "product location cannot be empty");
    owner o = {logger, user_itr->manager, time_point_sec(current_time_point())};
    location l = {logger, product_location, time_point_sec(current_time_point())};
    uint64_t product_id;
    products.emplace(get_self(), [&](auto &row) {
        row.id = get_next_id();
        product_id = row.id;
        row.tag = product_tag;
        row.breed = product_breed;
        row.origin = product_origin;
        row.born = time_point_sec(current_time_point());
        row.owners.push_back(o);
        row.locations.push_back(l);
    });
    tags.emplace(get_self(), [&](auto &row) {
        row.id = product_tag;
        row.product_id = product_id;
    });
}

void productlogger::logowner(uint64_t product_id, name current_owner, name new_owner)
{
    require_auth(current_owner);
    check(current_owner != new_owner, "current and new owner cannot be the same");
    auto product_itr = products.find(product_id);
    check(product_itr != products.end(), "that product id does not exist");
    string error_msg = new_owner.to_string() + " does not exist on chain";
    check(is_account(new_owner), error_msg.c_str());
    auto user_itr = users.find(new_owner.value);
    error_msg = new_owner.to_string() + " is not a user";
    check(user_itr != users.end(), error_msg.c_str());
    error_msg = new_owner.to_string() + " is not a manager";
    check(user_itr->manager == new_owner, error_msg.c_str());
    error_msg = current_owner.to_string() + " is not the current owner";
    check(product_itr->owners.back().owner == current_owner, error_msg.c_str());
    owner o = {current_owner, new_owner, time_point_sec(current_time_point())};
    products.modify(product_itr, get_self(), [&](auto &row) {
        row.owners.push_back(o);
    });
}

void productlogger::logrecord(uint64_t product_id, string description, name logger)
{
    require_auth(logger);
    auto product_itr = products.find(product_id);
    check(product_itr != products.end(), "that product id does not exist");
    auto user_itr = users.find(logger.value);
    string error_msg = logger.to_string() + " is not a user";
    check(user_itr != users.end(), error_msg.c_str());
    error_msg = logger.to_string() + " is not authorized to execute this action";
    check(product_itr->owners.back().owner == user_itr->manager, error_msg.c_str());
    record m = {logger, description, time_point_sec(current_time_point())};
    products.modify(product_itr, get_self(), [&](auto &row) {
        row.records.push_back(m);
    });
}

void productlogger::setproducttag(uint64_t product_id, uint64_t new_product_tag, name logger)
{
    require_auth(logger);
    auto product_itr = products.find(product_id);
    check(product_itr != products.end(), "that product id does not exist");
    auto new_tag_itr = tags.find(new_product_tag);
    check(new_tag_itr == tags.end(), "that product tag already exists");
    auto user_itr = users.find(logger.value);
    string error_msg = logger.to_string() + " is not a user";
    check(user_itr != users.end(), error_msg.c_str());
    error_msg = logger.to_string() + " is not authorized to execute this action";
    check(product_itr->owners.back().owner == user_itr->manager, error_msg.c_str());
    uint64_t current_product_tag = product_itr->tag;
    auto current_tag_itr = tags.find(current_product_tag);
    tags.erase(current_tag_itr);
    products.modify(product_itr, get_self(), [&](auto &row) {
        row.tag = new_product_tag;
    });
    tags.emplace(get_self(), [&](auto &row) {
        row.id = new_product_tag;
        row.product_id = product_id;
    });
}

void productlogger::sold(uint64_t product_id, name logger)
{
    require_auth(logger);
    auto product_itr = products.find(product_id);
    check(product_itr != products.end(), "that product id does not exist");
    check(!product_itr->consumed.status, "that product id has already been consumed");
    auto user_itr = users.find(logger.value);
    string error_msg = logger.to_string() + " is not a user";
    check(user_itr != users.end(), error_msg.c_str());
    error_msg = logger.to_string() + " is not authorized to execute this action";
    check(product_itr->owners.back().owner == user_itr->manager, error_msg.c_str());
    status sold = {logger, true, time_point_sec(current_time_point())};
    products.modify(product_itr, get_self(), [&](auto &row) {
        row.sold = sold;
    });
}

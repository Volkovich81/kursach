#include "../headers/CustomerService.h"
#include <algorithm>

CustomerService::~CustomerService() {
    // Пока не удаляем Customer, так как они управляются CinemaSystem
}

Customer* CustomerService::registerCustomer(const std::string& name, const std::string& email, const std::string& phone) {
    if (name.empty() || email.empty()) {
        throw ValidationException("Имя и email не могут быть пустыми");
    }

    // Проверяем, нет ли уже клиента с таким email
    for (Customer* customer : customers) {
        if (customer->getEmail() == email) {
            return customer;
        }
    }

    Customer* newCustomer = new Customer(nextCustomerId++, name, email, phone);
    customers.push_back(newCustomer);
    return newCustomer;
}

Customer* CustomerService::getCustomerById(int id) const {
    for (Customer* customer : customers) {
        if (customer->getCustomerId() == id) {
            return customer;
        }
    }
    throw CinemaException("Клиент с ID " + std::to_string(id) + " не найден");
}

Customer* CustomerService::getCustomerByEmail(const std::string& email) const {
    for (Customer* customer : customers) {
        if (customer->getEmail() == email) {
            return customer;
        }
    }
    return nullptr;
}

std::vector<Customer*> CustomerService::getAllCustomers() const {
    return customers;
}

bool CustomerService::updateCustomer(int id, const std::string& name, const std::string& email, const std::string& phone) {
    Customer* customer = getCustomerById(id);
    customer->setName(name);
    customer->setEmail(email);
    customer->setPhone(phone);
    return true;
}

double CustomerService::getCustomerLoyaltyPoints(int customerId) const {
    Customer* customer = getCustomerById(customerId);
    return customer->getLoyaltyPoints();
}
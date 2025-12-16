#ifndef CUSTOMERSERVICE_H
#define CUSTOMERSERVICE_H

#include <vector>
#include <string>
#include "Customer.h"
#include "CinemaException.h"

class CustomerService {
private:
    std::vector<Customer*> customers;
    int nextCustomerId = 1;

public:
    CustomerService() = default;
    ~CustomerService();

    Customer* registerCustomer(const std::string& name, const std::string& email, const std::string& phone);
    Customer* getCustomerById(int id) const;
    Customer* getCustomerByEmail(const std::string& email) const;
    std::vector<Customer*> getAllCustomers() const;
    bool updateCustomer(int id, const std::string& name, const std::string& email, const std::string& phone);
    double getCustomerLoyaltyPoints(int customerId) const;
};

#endif // CUSTOMERSERVICE_H

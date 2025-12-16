#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Person.h"

class Ticket;

class Customer : public Person {
private:
    int customerId;
    Ticket** bookingHistory;
    int bookingsCount;
    int maxBookings;
    double loyaltyPoints;

public:
    Customer(int id = 0, string n = "", string e = "", string p = "");
    ~Customer();

    void addToHistory(Ticket* ticket);
    int getBookingsCount() const;

    int getCustomerId() const;
    double getLoyaltyPoints() const;
};

#endif
#ifndef BOOKING_H
#define BOOKING_H

#include "Customer.h"
#include "Ticket.h"
#include <string>
using namespace std;

class Booking {
private:
    static int nextId;
    int bookingId;
    Customer* customer;
    Ticket** tickets;
    int ticketsCount;
    int maxTickets;
    string bookingDate;
    string status;

public:
    Booking(Customer* cust = nullptr);
    ~Booking();

    void addTicket(Ticket* ticket);
    void confirmBooking();
    void cancelBooking();
    double calculateTotal() const;
    void displayBooking() const;

    int getBookingId() const;
    Customer* getCustomer() const;
    int getTicketsCount() const;
    string getStatus() const;
};

#endif


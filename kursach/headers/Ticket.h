#ifndef TICKET_H
#define TICKET_H

class Session;
class Customer;

class Ticket {
private:
    static int nextId;
    int ticketId;
    Session* session;
    Customer* customer;
    int seatNumber;
    bool isBooked;

public:
    Ticket(Session* s = nullptr, Customer* c = nullptr, int seat = 0);

    void bookTicket();
    void displayTicket() const;

    int getTicketId() const;
    Session* getSession() const;
    Customer* getCustomer() const;
    int getSeatNumber() const;
    bool getIsBooked() const;
};

#endif
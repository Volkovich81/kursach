#include "../headers/Booking.h"
#include "../headers/Session.h" 
#include "../headers/Ticket.h" 
#include <iostream>
using namespace std;

int Booking::nextId = 1;

Booking::Booking(Customer* cust)
    : customer(cust), tickets(nullptr), ticketsCount(0), maxTickets(10), status("pending") {
    bookingId = nextId++;
    tickets = new Ticket * [maxTickets];
    for (int i = 0; i < maxTickets; i++) {
        tickets[i] = nullptr;
    }
}

Booking::~Booking() {
    delete[] tickets;
}

void Booking::addTicket(Ticket* ticket) {
    if (ticketsCount < maxTickets) {
        tickets[ticketsCount] = ticket;
        ticketsCount++;
    }
}

void Booking::confirmBooking() {
    status = "confirmed";
    for (int i = 0; i < ticketsCount; i++) {
        if (tickets[i] != nullptr) {
            tickets[i]->bookTicket();
        }
    }
}

void Booking::cancelBooking() {
    status = "cancelled";
}

double Booking::calculateTotal() const {
    double total = 0.0;
    for (int i = 0; i < ticketsCount; i++) {
        if (tickets[i] != nullptr && tickets[i]->getSession() != nullptr) {
            total += tickets[i]->getSession()->getTicketPrice();
        }
    }
    return total;
}

void Booking::displayBooking() const {
    cout << "=== Booking ===" << endl;
    cout << "Booking ID: " << bookingId << endl;
    if (customer != nullptr) {
        cout << "Customer: " << customer->getName() << endl;
    }
    cout << "Tickets Count: " << ticketsCount << endl;
    cout << "Total: $" << calculateTotal() << endl;
    cout << "Status: " << status << endl;

    for (int i = 0; i < ticketsCount; i++) {
        if (tickets[i] != nullptr) {
            tickets[i]->displayTicket();
        }
    }
}

int Booking::getBookingId() const { return bookingId; }
Customer* Booking::getCustomer() const { return customer; }
int Booking::getTicketsCount() const { return ticketsCount; }
string Booking::getStatus() const { return status; }
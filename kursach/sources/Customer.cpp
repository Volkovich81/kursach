#include "../headers/Customer.h"
#include "../headers/Ticket.h"

#define INITIAL_BOOKINGS 10

Customer::Customer(int id, string n, string e, string p)
    : Person(n, e, p), customerId(id), bookingsCount(0),
    maxBookings(INITIAL_BOOKINGS), loyaltyPoints(0), bookingHistory(nullptr) {
    bookingHistory = new Ticket * [maxBookings];
    for (int i = 0; i < maxBookings; i++) {
        bookingHistory[i] = nullptr;
    }
}

Customer::~Customer() {
    delete[] bookingHistory;
}

void Customer::addToHistory(Ticket* ticket) {
    if (bookingsCount < maxBookings) {
        bookingHistory[bookingsCount] = ticket;
        bookingsCount++;
        loyaltyPoints += 10;
    }
}

int Customer::getCustomerId() const { return customerId; }
double Customer::getLoyaltyPoints() const { return loyaltyPoints; }
int Customer::getBookingsCount() const { return bookingsCount; }
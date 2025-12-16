#include "../headers/Ticket.h"
#include "../headers/Session.h"
#include "../headers/Customer.h"
#include <iostream>
using namespace std;

int Ticket::nextId = 1;

Ticket::Ticket(Session* s, Customer* c, int seat)
    : session(s), customer(c), seatNumber(seat), isBooked(false) {
    ticketId = nextId++;
}

void Ticket::bookTicket() {
    if (session != nullptr && session->isSeatAvailable(seatNumber)) {
        if (session->bookSeat(seatNumber)) {
            isBooked = true;
            if (customer != nullptr) {
                customer->addToHistory(this);
            }
        }
    }
}

void Ticket::displayTicket() const {
    cout << "=== Ticket ===" << endl;
    cout << "Ticket ID: " << ticketId << endl;
    cout << "Seat: " << seatNumber << endl;
    if (session != nullptr && session->getMovie() != nullptr && session->getHall() != nullptr) {
        cout << "Movie: " << session->getMovie()->getTitle() << endl;
        cout << "Hall: " << session->getHall()->getHallNumber() << endl;
        cout << "Time: " << session->getDateTime() << endl;
        cout << "Price: $" << session->getTicketPrice() << endl;
    }
    cout << "Status: " << (isBooked ? "Booked" : "Pending") << endl;
}

int Ticket::getTicketId() const { return ticketId; }
Session* Ticket::getSession() const { return session; }
Customer* Ticket::getCustomer() const { return customer; }
int Ticket::getSeatNumber() const { return seatNumber; }
bool Ticket::getIsBooked() const { return isBooked; }
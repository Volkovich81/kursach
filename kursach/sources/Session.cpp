#include "../headers/Session.h"

int Session::nextId = 1;

Session::Session(Movie* m, CinemaHall* h, string dt, double price)
    : movie(m), hall(h), dateTime(dt), ticketPrice(price) {
    sessionId = nextId++;
}

bool Session::bookSeat(int seatNumber) {
    if (hall != nullptr) {
        return hall->bookSeat(seatNumber);
    }
    return false;
}

bool Session::isSeatAvailable(int seatNumber) const {
    if (hall != nullptr) {
        return hall->isSeatAvailable(seatNumber);
    }
    return false;
}

int Session::getSessionId() const { return sessionId; }
Movie* Session::getMovie() const { return movie; }
CinemaHall* Session::getHall() const { return hall; }
string Session::getDateTime() const { return dateTime; }
double Session::getTicketPrice() const { return ticketPrice; }

void Session::setMovie(Movie* m) { movie = m; }
void Session::setHall(CinemaHall* h) { hall = h; }
void Session::setDateTime(string dt) { dateTime = dt; }
void Session::setTicketPrice(double price) { ticketPrice = price; }
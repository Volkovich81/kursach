#ifndef SESSION_H
#define SESSION_H

#include "Movie.h"
#include "CinemaHall.h"
#include <string>
using namespace std;

class Session {
private:
    static int nextId;
    int sessionId;
    Movie* movie;
    CinemaHall* hall;
    string dateTime;
    double ticketPrice;

public:
    Session(Movie* m = nullptr, CinemaHall* h = nullptr, string dt = "", double price = 0.0);

    bool bookSeat(int seatNumber);
    bool isSeatAvailable(int seatNumber) const;

    int getSessionId() const;
    Movie* getMovie() const;
    CinemaHall* getHall() const;
    string getDateTime() const;
    double getTicketPrice() const;

    void setMovie(Movie* m);
    void setHall(CinemaHall* h);
    void setDateTime(string dt);
    void setTicketPrice(double price);
};

#endif

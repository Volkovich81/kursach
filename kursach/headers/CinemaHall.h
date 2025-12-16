#ifndef CINEMAHALL_H
#define CINEMAHALL_H

class CinemaHall {
private:
    int hallNumber;
    int totalSeats;
    bool* seats;

public:
    CinemaHall(int number = 0, int seatsCount = 50);
    ~CinemaHall();

    bool bookSeat(int seatNumber);
    bool isSeatAvailable(int seatNumber) const;
    int getAvailableSeatsCount() const;

    int getHallNumber() const;
    int getTotalSeats() const;
};

#endif

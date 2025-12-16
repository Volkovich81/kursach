#include "../headers/CinemaHall.h"

CinemaHall::CinemaHall(int number, int seatsCount)
    : hallNumber(number), totalSeats(seatsCount), seats(nullptr) {
    seats = new bool[totalSeats];
    for (int i = 0; i < totalSeats; i++) {
        seats[i] = false;
    }
}

CinemaHall::~CinemaHall() {
    delete[] seats;
}

bool CinemaHall::bookSeat(int seatNumber) {
    if (seatNumber >= 0 && seatNumber < totalSeats && !seats[seatNumber]) {
        seats[seatNumber] = true;
        return true;
    }
    return false;
}

bool CinemaHall::isSeatAvailable(int seatNumber) const {
    return (seatNumber >= 0 && seatNumber < totalSeats && !seats[seatNumber]);
}

int CinemaHall::getAvailableSeatsCount() const {
    int count = 0;
    for (int i = 0; i < totalSeats; i++) {
        if (!seats[i]) count++;
    }
    return count;
}

int CinemaHall::getHallNumber() const { return hallNumber; }
int CinemaHall::getTotalSeats() const { return totalSeats; }
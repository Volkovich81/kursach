#ifndef CINEMASYSTEM_H
#define CINEMASYSTEM_H

#include "Movie.h"
#include "CinemaHall.h"
#include "Session.h"
#include "Customer.h"
#include "Booking.h"
#include "Ticket.h"
#include "MovieService.h"
#include "SessionService.h"
#include <string>
#include <vector>
#include <iostream>
#include "BookingService.h"
#include "CustomerService.h"
using namespace std;

class CinemaSystem {
private:
    static const int MAX_MOVIES = 50;
    static const int MAX_HALLS = 10;
    static const int MAX_SESSIONS = 100;
    static const int MAX_CUSTOMERS = 200;
    static const int MAX_BOOKINGS = 500;

    Customer* customers[MAX_CUSTOMERS];
    Booking* bookings[MAX_BOOKINGS];

    MovieService movieService;
    SessionService sessionService;
    BookingService bookingService;
    CustomerService customerService;

    int customersCount;
    int bookingsCount;

public:
    Movie* movies[MAX_MOVIES];
    CinemaHall* halls[MAX_HALLS];
    Session* sessions[MAX_SESSIONS];
    int moviesCount;
    int hallsCount;
    int sessionsCount;
    SessionService& getSessionService() { return sessionService; }

    CinemaSystem();

    void addMovie(Movie* movie);
    void showAllMovies() const;
    void addHall(CinemaHall* hall);
    void createSession(Movie* movie, CinemaHall* hall, string datetime, double price, Session* excludeSession = nullptr);
    void showAllSessions() const;
    Customer* registerCustomer(string name, string email, string phone);
    Booking* createBooking(Customer* customer);
    void findSessionsByMovie(string movieTitle) const;
    void showAvailableSeats(int sessionId) const;
    void runDemo();

    vector<Movie*> getAllMovies() const;

    // Только объявления методов, реализации в .cpp
    Session* getSessionById(int sessionId) const;
    bool isSeatAvailable(int sessionId, int seatNumber) const;
    Booking* bookTicket(int sessionId, int seatNumber, const string& customerName, const string& customerEmail);

    std::vector<Session*> getPastSessions() const;

    BookingService& getBookingService() { return bookingService; }
    CustomerService& getCustomerService() { return customerService; }
    MovieService& getMovieService() { return movieService; }
};

#endif

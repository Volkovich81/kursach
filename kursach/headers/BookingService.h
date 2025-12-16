#ifndef BOOKINGSERVICE_H
#define BOOKINGSERVICE_H

#include <vector>
#include "Booking.h"
#include "CinemaException.h"

class BookingService {
private:
    std::vector<Booking*> bookings;

public:
    BookingService() = default;
    ~BookingService();

    void addBooking(Booking* booking);
    Booking* getBookingById(int id) const;
    std::vector<Booking*> getAllBookings() const;
    std::vector<Booking*> getBookingsByCustomer(int customerId) const;
    bool cancelBooking(int bookingId);
    double calculateTotalRevenue() const;
};

#endif // BOOKINGSERVICE_H
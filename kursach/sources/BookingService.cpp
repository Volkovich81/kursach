#include "../headers/BookingService.h"
#include <algorithm>

BookingService::~BookingService() {
    // Пока не удаляем Booking, так как они управляются CinemaSystem
}

void BookingService::addBooking(Booking* booking) {
    if (!booking) {
        throw ValidationException("Бронирование не может быть пустым");
    }
    bookings.push_back(booking);
}

Booking* BookingService::getBookingById(int id) const {
    for (Booking* booking : bookings) {
        if (booking->getBookingId() == id) {
            return booking;
        }
    }
    throw CinemaException("Бронирование с ID " + std::to_string(id) + " не найдено");
}

std::vector<Booking*> BookingService::getAllBookings() const {
    return bookings;
}

std::vector<Booking*> BookingService::getBookingsByCustomer(int customerId) const {
    std::vector<Booking*> result;
    for (Booking* booking : bookings) {
        if (booking->getCustomer() && booking->getCustomer()->getCustomerId() == customerId) {
            result.push_back(booking);
        }
    }
    return result;
}

bool BookingService::cancelBooking(int bookingId) {
    Booking* booking = getBookingById(bookingId);
    booking->cancelBooking();
    return true;
}

double BookingService::calculateTotalRevenue() const {
    double total = 0.0;
    for (Booking* booking : bookings) {
        if (booking->getStatus() == "confirmed") {
            total += booking->calculateTotal();
        }
    }
    return total;
}
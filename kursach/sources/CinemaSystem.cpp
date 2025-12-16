#include "../headers/CinemaSystem.h"
#include "../headers/Ticket.h" 
#include "../headers/DateUtils.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

CinemaSystem::CinemaSystem() {
    moviesCount = 0;
    hallsCount = 0;
    sessionsCount = 0;
    customersCount = 0;
    bookingsCount = 0;

    for (int i = 0; i < MAX_MOVIES; i++) movies[i] = nullptr;
    for (int i = 0; i < MAX_HALLS; i++) halls[i] = nullptr;
    for (int i = 0; i < MAX_SESSIONS; i++) sessions[i] = nullptr;
    for (int i = 0; i < MAX_CUSTOMERS; i++) customers[i] = nullptr;
    for (int i = 0; i < MAX_BOOKINGS; i++) bookings[i] = nullptr;

    addMovie(new Movie("Тестовый фильм", "Драма", 120, "Тест режиссер", 12));

    addMovie(new Movie("Аватар", "Фантастика", 162, "Джеймс Кэмерон", 12));
    addMovie(new Movie("Форсаж", "Боевик", 137, "Джастин Лин", 16));
    addMovie(new Movie("Титаник", "Драма", 195, "Джеймс Кэмерон", 12));
    addMovie(new Movie("Начало", "Фантастика", 148, "Кристофер Нолан", 12));
    addMovie(new Movie("Хатико", "Драма", 93, "Лассе Халльстрём", 6));

    addHall(new CinemaHall(1, 50));
    addHall(new CinemaHall(2, 30));

    createSession(movies[0], halls[0], "2023-12-01 18:00", 350.0);  // Точно прошедшая
    createSession(movies[1], halls[1], "2023-11-15 20:30", 400.0);  // Точно прошедшая
    createSession(movies[2], halls[0], "2023-12-01 14:00", 300.0); // Точно прошедший

}

void CinemaSystem::addMovie(Movie* movie) {
    try {
        // Добавляем через сервис
        movieService.addMovie(movie);

        // Также сохраняем в старый массив для обратной совместимости
        if (moviesCount < MAX_MOVIES) {
            movies[moviesCount] = movie;
            moviesCount++;
            cout << "Movie added: " << movie->getTitle() << endl;
        }
    }
    catch (const CinemaException& e) {
        cout << "Error adding movie: " << e.what() << endl;
        throw;
    }
}

void CinemaSystem::showAllMovies() const {
    cout << "=== All Movies ===" << endl;
    for (int i = 0; i < moviesCount; i++) {
        if (movies[i] != nullptr) {
            cout << "ID: " << movies[i]->getId()
                << " | Title: " << movies[i]->getTitle()
                << " | Genre: " << movies[i]->getGenre()
                << " | Duration: " << movies[i]->getDuration() << "min" << endl;
        }
    }
    cout << "==================" << endl;
}

void CinemaSystem::addHall(CinemaHall* hall) {
    if (hallsCount < MAX_HALLS) {
        halls[hallsCount] = hall;
        hallsCount++;
        cout << "Hall added: " << hall->getHallNumber() << endl;
    }
}

void CinemaSystem::createSession(Movie* movie, CinemaHall* hall, string datetime, double price, Session* excludeSession)
{
    try {
        QDateTime newDateTime = QDateTime::fromString(QString::fromStdString(datetime), "yyyy-MM-dd HH:mm");

        // ★★★ ПЕРЕДАЕМ excludeSession ★★★
        if (!sessionService.isHallAvailable(hall->getHallNumber(), newDateTime, movie->getDuration(), excludeSession)) {
            throw BusinessLogicException("Зал уже занят в это время!");
        }

        // Просто создаем сеанс
        if (sessionsCount < MAX_SESSIONS) {
            sessions[sessionsCount] = new Session(movie, hall, datetime, price);
            sessionService.addSession(sessions[sessionsCount]);
            sessionsCount++;
            cout << "Session created for movie: " << movie->getTitle() << endl;
        }
    }
    catch (const CinemaException& e) {
        cout << "Error creating session: " << e.what() << endl;
        throw;
    }
}

void CinemaSystem::showAllSessions() const {
    cout << "=== All Sessions ===" << endl;
    for (int i = 0; i < sessionsCount; i++) {
        if (sessions[i] != nullptr && sessions[i]->getMovie() != nullptr) {
            cout << "Session ID: " << sessions[i]->getSessionId()
                << " | Movie: " << sessions[i]->getMovie()->getTitle()
                << " | Hall: " << sessions[i]->getHall()->getHallNumber()
                << " | Time: " << sessions[i]->getDateTime()
                << " | Price: $" << sessions[i]->getTicketPrice() << endl;
        }
    }
    cout << "====================" << endl;
}

Customer* CinemaSystem::registerCustomer(string name, string email, string phone) {
    if (customersCount < MAX_CUSTOMERS) {
        customers[customersCount] = new Customer(customersCount + 1, name, email, phone);
        customersCount++;
        cout << "Customer registered: " << name << endl;
        return customers[customersCount - 1];
    }
    return nullptr;
}

Booking* CinemaSystem::createBooking(Customer* customer) {
    if (bookingsCount < MAX_BOOKINGS) {
        bookings[bookingsCount] = new Booking(customer);
        bookingsCount++;
        return bookings[bookingsCount - 1];
    }
    return nullptr;
}

void CinemaSystem::findSessionsByMovie(string movieTitle) const {
    cout << "=== Sessions for movie: " << movieTitle << " ===" << endl;
    for (int i = 0; i < sessionsCount; i++) {
        if (sessions[i] != nullptr &&
            sessions[i]->getMovie() != nullptr &&
            sessions[i]->getMovie()->getTitle() == movieTitle) {
            cout << "Session ID: " << sessions[i]->getSessionId()
                << " | Hall: " << sessions[i]->getHall()->getHallNumber()
                << " | Time: " << sessions[i]->getDateTime() << endl;
        }
    }
}

void CinemaSystem::showAvailableSeats(int sessionId) const {
    for (int i = 0; i < sessionsCount; i++) {
        if (sessions[i] != nullptr && sessions[i]->getSessionId() == sessionId) {
            CinemaHall* hall = sessions[i]->getHall();
            if (hall != nullptr) {
                cout << "Available seats for session " << sessionId << ":" << endl;
                for (int seat = 0; seat < hall->getTotalSeats(); seat++) {
                    if (hall->isSeatAvailable(seat)) {
                        cout << "Seat " << seat << " ";
                    }
                }
                cout << endl;
            }
        }
    }
}

void CinemaSystem::runDemo() {
    cout << "\n=== Running Cinema System Demo ===" << endl;

    Movie* movie1 = new Movie("Avatar", "Fantasy", 162, "James Cameron", 12);
    Movie* movie2 = new Movie("Fast and Furious", "Action", 137, "Justin Lin", 16);
    addMovie(movie1);
    addMovie(movie2);

    CinemaHall* hall1 = new CinemaHall(1, 20);
    CinemaHall* hall2 = new CinemaHall(2, 15);
    addHall(hall1);
    addHall(hall2);

    createSession(movie1, hall1, "2024-01-15 18:00", 10.5);
    createSession(movie2, hall2, "2024-01-15 20:30", 12.0);

    Customer* customer = registerCustomer("John Doe", "john@email.com", "123456789");

    showAllMovies();
    showAllSessions();

    Booking* booking = createBooking(customer);
    if (booking != nullptr && sessions[0] != nullptr) {
        Ticket* ticket1 = new Ticket(sessions[0], customer, 5);
        Ticket* ticket2 = new Ticket(sessions[0], customer, 6);
        booking->addTicket(ticket1);
        booking->addTicket(ticket2);
        booking->confirmBooking();

        cout << "\n=== Booking Details ===" << endl;
        booking->displayBooking();
    }

    cout << "\n=== Demo Completed Successfully! ===" << endl;
}
vector<Movie*> CinemaSystem::getAllMovies() const {
    return movieService.getAllMovies();
}

Session* CinemaSystem::getSessionById(int sessionId) const {
    for (int i = 0; i < sessionsCount; i++) {
        if (sessions[i] != nullptr && sessions[i]->getSessionId() == sessionId) {
            return sessions[i];
        }
    }
    return nullptr;
}

bool CinemaSystem::isSeatAvailable(int sessionId, int seatNumber) const {
    Session* session = getSessionById(sessionId);
    if (session != nullptr) {
        return session->isSeatAvailable(seatNumber);
    }
    return false;
}

Booking* CinemaSystem::bookTicket(int sessionId, int seatNumber, const string& customerName, const string& customerEmail) {

    Session* session = getSessionById(sessionId);
    if (session == nullptr) {
        cout << "Session not found!" << endl;
        return nullptr;
    }

    if (!session->isSeatAvailable(seatNumber)) {
        cout << "Seat " << seatNumber << " is not available!" << endl;
        return nullptr;
    }

    Customer* customer = nullptr;

    for (int i = 0; i < customersCount; i++) {
        if (customers[i] != nullptr && customers[i]->getEmail() == customerEmail) {
            customer = customers[i];
            break;
        }
    }

    if (customer == nullptr) {
        customer = registerCustomer(customerName, customerEmail, "0000000000");
        if (customer == nullptr) {
            cout << "Failed to register customer!" << endl;
            return nullptr;
        }
    }

    Booking* booking = createBooking(customer);
    if (booking == nullptr) {
        cout << "Failed to create booking!" << endl;
        return nullptr;
    }

    Ticket* ticket = new Ticket(session, customer, seatNumber);
    booking->addTicket(ticket);

    booking->confirmBooking();

    cout << "Ticket booked successfully!" << endl;
    return booking;
}

std::vector<Session*> CinemaSystem::getPastSessions() const {
    std::vector<Session*> result;
    for (int i = 0; i < sessionsCount; i++) {
        Session* session = sessions[i];
        if (session != nullptr && session->getMovie() != nullptr) {
            if (DateUtils::isPast(QString::fromStdString(session->getDateTime()))) {
                result.push_back(session);
            }
        }
    }
    return result;
}
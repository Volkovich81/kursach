#ifndef CINEMAEXCEPTION_H
#define CINEMAEXCEPTION_H

#include <stdexcept>
#include <string>
#include <QString>

class CinemaException : public std::exception {
private:
    std::string message;

public:
    CinemaException(const std::string& msg) : message(msg) {}
    CinemaException(const QString& msg) : message(msg.toStdString()) {}
    CinemaException(const char* msg) : message(msg) {}  // Добавили конструктор для строковых литералов

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ValidationException : public CinemaException {
public:
    ValidationException(const std::string& msg) : CinemaException("Ошибка валидации: " + msg) {}
    ValidationException(const QString& msg) : CinemaException("Ошибка валидации: " + msg) {}
    ValidationException(const char* msg) : CinemaException(std::string("Ошибка валидации: ") + msg) {}  // Добавили для строковых литералов
};

class BusinessLogicException : public CinemaException {
public:
    BusinessLogicException(const std::string& msg) : CinemaException("Ошибка бизнес-логики: " + msg) {}
    BusinessLogicException(const QString& msg) : CinemaException("Ошибка бизнес-логики: " + msg) {}
    BusinessLogicException(const char* msg) : CinemaException(std::string("Ошибка бизнес-логики: ") + msg) {}  // Добавили для строковых литералов
};

#endif
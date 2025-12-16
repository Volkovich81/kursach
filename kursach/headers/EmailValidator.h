#ifndef EMAILVALIDATOR_H
#define EMAILVALIDATOR_H

#include <QString>
#include <QRegularExpression>

class EmailValidator {
public:
    static bool isValid(const QString& email) {
        QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        return regex.match(email).hasMatch();
    }
};

#endif

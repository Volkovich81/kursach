#ifndef DATEUTILS_H
#define DATEUTILS_H

#include <QString>
#include <QDateTime>
#include <QDebug>

class DateUtils {
public:
    static bool isPast(const QString& dateTimeStr) {
        QDateTime sessionTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd HH:mm");
        if (!sessionTime.isValid()) {
            return false;
        }
        return sessionTime < QDateTime::currentDateTime();
    }

    static bool isFuture(const QString& dateTimeStr) {
        QDateTime sessionTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd HH:mm");
        if (!sessionTime.isValid()) {
            return true;
        }
        return sessionTime > QDateTime::currentDateTime();
    }
};

#endif
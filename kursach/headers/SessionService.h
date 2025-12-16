#ifndef SESSIONSERVICE_H
#define SESSIONSERVICE_H

#include "Session.h"
#include "CinemaException.h"
#include <vector>
#include <QDateTime>
#include <algorithm>

class SessionService {
private:
    std::vector<Session*> sessions;

public:
    ~SessionService();

    void addSession(Session* session);
    Session* getSessionById(int id) const;
    std::vector<Session*> getAllSessions() const;

    // ★★★ ОБНОВЛЯЕМ МЕТОД ДЛЯ ИСКЛЮЧЕНИЯ СЕАНСА ★★★
    bool isHallAvailable(int hallNumber, const QDateTime& newDateTime, int movieDuration, Session* excludeSession = nullptr) const;

    // Получение сеансов по залу
    std::vector<Session*> getSessionsByHall(int hallNumber) const;

private:
    bool hasTimeOverlap(const Session* existingSession, const QDateTime& newDateTime, int newDuration) const;
    QDateTime parseDateTime(const std::string& datetimeStr) const;
};

#endif
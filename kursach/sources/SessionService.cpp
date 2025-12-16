#include "../headers/SessionService.h"
#include <QDateTime>

SessionService::~SessionService() {
    // Пока не удаляем Session, так как они управляются CinemaSystem
}

void SessionService::addSession(Session* session) {
    if (!session) {
        throw ValidationException("Session cannot be null");
    }
    sessions.push_back(session);
}

Session* SessionService::getSessionById(int id) const {
    for (Session* session : sessions) {
        if (session->getSessionId() == id) {
            return session;
        }
    }
    throw CinemaException("Session not found with id: " + std::to_string(id));
}

std::vector<Session*> SessionService::getAllSessions() const {
    return sessions;
}

std::vector<Session*> SessionService::getSessionsByHall(int hallNumber) const {
    std::vector<Session*> result;
    for (Session* session : sessions) {
        if (session->getHall() && session->getHall()->getHallNumber() == hallNumber) {
            result.push_back(session);
        }
    }
    return result;
}

// ★★★ УБИРАЕМ значение по умолчанию В РЕАЛИЗАЦИИ ★★★
bool SessionService::isHallAvailable(int hallNumber, const QDateTime& newDateTime, int movieDuration, Session* excludeSession) const {
    std::vector<Session*> hallSessions = getSessionsByHall(hallNumber);

    for (Session* existingSession : hallSessions) {
        // ИСКЛЮЧАЕМ ТЕКУЩИЙ РЕДАКТИРУЕМЫЙ СЕАНС
        if (excludeSession && existingSession->getSessionId() == excludeSession->getSessionId()) {
            continue;
        }

        if (hasTimeOverlap(existingSession, newDateTime, movieDuration)) {
            return false;
        }
    }

    return true;
}

bool SessionService::hasTimeOverlap(const Session* existingSession, const QDateTime& newDateTime, int newDuration) const {
    if (!existingSession || !existingSession->getMovie()) {
        return false;
    }

    QDateTime existingStart = parseDateTime(existingSession->getDateTime());
    if (!existingStart.isValid()) {
        return false;
    }

    int existingDuration = existingSession->getMovie()->getDuration();
    QDateTime existingEnd = existingStart.addSecs(existingDuration * 60);

    QDateTime newStart = newDateTime;
    QDateTime newEnd = newStart.addSecs(newDuration * 60);

    // ★★★ САМАЯ ПРОСТАЯ ПРОВЕРКА ★★★
    // Пересечение есть если новый сеанс начинается раньше чем закончится старый + 15 мин
    // И новый сеанс заканчивается позже чем начался старый
    return (newStart < existingEnd.addSecs(15 * 60)) && (newEnd > existingStart);
}

QDateTime SessionService::parseDateTime(const std::string& datetimeStr) const {
    QDateTime dt = QDateTime::fromString(QString::fromStdString(datetimeStr), "yyyy-MM-dd HH:mm");
    if (!dt.isValid()) {
        qDebug() << "Invalid datetime string:" << QString::fromStdString(datetimeStr);
    }
    return dt;
}
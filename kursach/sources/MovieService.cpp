#include "../headers/MovieService.h"
#include <algorithm>
#include <string>

// Деструктор - очищаем память
MovieService::~MovieService() {
    // Пока не удаляем Movie, так как они управляются CinemaSystem
    // В будущем можно будет перейти на умные указатели
}

// Добавление фильма с валидацией
void MovieService::addMovie(Movie* movie) {
    if (!movie) {
        throw ValidationException("Фильм не может быть пустым");
    }
    validateMovie(movie);
    movies.push_back(movie);
}

// Поиск фильма по ID
Movie* MovieService::getMovieById(int id) {
    for (Movie* movie : movies) {
        if (movie->getId() == id) {
            return movie;
        }
    }
    throw CinemaException("Фильм с ID " + std::to_string(id) + " не найден");
}

// Получение всех фильмов
std::vector<Movie*> MovieService::getAllMovies() const {
    std::vector<Movie*> result;
    for (Movie* movie : movies) {
        if (movie != nullptr) {  // ← ДОБАВИТЬ ЭТУ ПРОВЕРКУ
            result.push_back(movie);
        }
    }
    return result;
}
// Обновление фильма
bool MovieService::updateMovie(int id, const std::string& title, const std::string& genre,
    int duration, const std::string& director, int ageRating) {
    Movie* movie = getMovieById(id);

    // Проверяем, есть ли сеансы у этого фильма
    if (hasSessions(id)) {
        throw BusinessLogicException("Нельзя редактировать фильм, для которого есть сеансы");
    }

    // Обновляем данные
    movie->setTitle(title);
    movie->setGenre(genre);
    movie->setDuration(duration);
    movie->setDirector(director);
    movie->setAgeRating(ageRating);

    return true;
}

// Удаление фильма
bool MovieService::deleteMovie(int id) {
    for (auto it = movies.begin(); it != movies.end(); ++it) {
        if (*it && (*it)->getId() == id) {
            movies.erase(it);
            return true;
        }
    }
    return false;
}

// Проверка наличия сеансов у фильма (пока заглушка)
bool MovieService::hasSessions(int movieId) const {
    // TODO: Реализовать проверку через SessionService
    // Пока возвращаем false для тестирования
    return false;
}

// Поиск фильмов по названию
std::vector<Movie*> MovieService::searchByTitle(const std::string& title) const {
    std::vector<Movie*> result;
    std::string lowerTitle = title;

    // Преобразуем поисковый запрос в нижний регистр
    std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);

    for (Movie* movie : movies) {
        std::string movieTitle = movie->getTitle();
        std::transform(movieTitle.begin(), movieTitle.end(), movieTitle.begin(), ::tolower);

        // Если название содержит искомую строку
        if (movieTitle.find(lowerTitle) != std::string::npos) {
            result.push_back(movie);
        }
    }
    return result;
}

// Валидация данных фильма
void MovieService::validateMovie(const Movie* movie) const {
    if (movie->getTitle().empty()) {
        throw ValidationException("Название фильма не может быть пустым");
    }
    if (movie->getDuration() <= 0) {
        throw ValidationException("Продолжительность фильма должна быть положительной");
    }
    if (movie->getAgeRating() < 0) {
        throw ValidationException("Возрастной рейтинг не может быть отрицательным");
    }
}
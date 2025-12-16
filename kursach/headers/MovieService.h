#ifndef MOVIESERVICE_H
#define MOVIESERVICE_H

#include "CinemaException.h"
#include "Movie.h"
#include <vector>
#include <algorithm>

class MovieService {
private:
    std::vector<Movie*> movies;

public:
    // Деструктор для очистки памяти
    ~MovieService();

    // Основные методы работы с фильмами
    void addMovie(Movie* movie);
    Movie* getMovieById(int id);
    std::vector<Movie*> getAllMovies() const;
    bool updateMovie(int id, const std::string& title, const std::string& genre,
        int duration, const std::string& director, int ageRating);
    bool deleteMovie(int id);

    // Проверка, есть ли у фильма сеансы (пока заглушка)
    bool hasSessions(int movieId) const;

    // Поиск фильмов по названию
    std::vector<Movie*> searchByTitle(const std::string& title) const;

private:
    // Валидация данных фильма
    void validateMovie(const Movie* movie) const;
};

#endif

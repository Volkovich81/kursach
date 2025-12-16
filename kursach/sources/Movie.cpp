#include "../headers/Movie.h"

int Movie::nextId = 1;

Movie::Movie(string t, string g, int d, string dir, int age)
    : title(t), genre(g), duration(d), director(dir), ageRating(age) {
    movieId = nextId++;
}

int Movie::getId() const { return movieId; }
string Movie::getTitle() const { return title; }
string Movie::getGenre() const { return genre; }
int Movie::getDuration() const { return duration; }
string Movie::getDirector() const { return director; }
int Movie::getAgeRating() const { return ageRating; }

void Movie::setTitle(string t) { title = t; }
void Movie::setGenre(string g) { genre = g; }
void Movie::setDuration(int d) { duration = d; }
void Movie::setDirector(string dir) { director = dir; }
void Movie::setAgeRating(int age) { ageRating = age; }
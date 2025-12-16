#ifndef MOVIE_H
#define MOVIE_H

#include <string>
using namespace std;

class Movie {
private:
    static int nextId;
    int movieId;
    string title;
    string genre;
    int duration;
    string director;
    int ageRating;

public:
    Movie(string t = "", string g = "", int d = 0, string dir = "", int age = 0);

    int getId() const;
    string getTitle() const;
    string getGenre() const;
    int getDuration() const;
    string getDirector() const;
    int getAgeRating() const;

    void setTitle(string t);
    void setGenre(string g);
    void setDuration(int d);
    void setDirector(string dir);
    void setAgeRating(int age);
};

#endif

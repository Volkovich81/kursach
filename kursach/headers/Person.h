#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class Person {
protected:
    string name;
    string email;
    string phone;

public:
    Person(string n = "", string e = "", string p = "");

    string getName() const;
    string getEmail() const;
    string getPhone() const;

    void setName(string n);
    void setEmail(string e);
    void setPhone(string p);
};

#endif
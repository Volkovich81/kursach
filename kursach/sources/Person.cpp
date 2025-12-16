#include "../headers/Person.h"

Person::Person(string n, string e, string p)
    : name(n), email(e), phone(p) {
}

string Person::getName() const { return name; }
string Person::getEmail() const { return email; }
string Person::getPhone() const { return phone; }

void Person::setName(string n) { name = n; }
void Person::setEmail(string e) { email = e; }
void Person::setPhone(string p) { phone = p; }
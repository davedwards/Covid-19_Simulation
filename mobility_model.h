#ifndef MOBILITY_MODEL_H
#define MOBILITY_MODEL_H

class Person;

class MobilityModel {
protected:
    Person *person;
public:
    MobilityModel();
    void setPerson(Person* p);
    virtual void move() = 0;
};

#endif
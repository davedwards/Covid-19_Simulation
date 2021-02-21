#ifndef PERSON_H
#define PERSON_H

#include "simulator.h"
#include "mobility_model.h"
#include "location.h"

class MobilityModel;


class Person {
    public:
        Person();
        bool infect();
        bool try_infect(Person other_person);
        void progress_disease();
        bool is_alive();

        Location location;
        MobilityModel *mobility_model;
        disease_status status;
        int disease_counter;
};

#endif
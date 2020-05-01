#include "person.h"
#include "ppmodel.h"
// #include "rwpmodel.h"


Person::Person() 
{
    // RandomWaypointModel *model = new RandomwaypointModel();
    PopularPlacesModel *model = new PopularPlacesModel();
    mobility_model = (MobilityModel*) model;
    mobility_model->setPerson(this);
    status = VULNERABLE;
    disease_counter = INFECTION_TIME;
}

bool Person::infect() {
    if (status == VULNERABLE){
        status = INFECTED;
        return true;
    } else {
        return false;
    }
}

bool Person::try_infect(Person other_person){
    if (other_person.status != INFECTED){
        return false;
    }
    if (location.get_distance(other_person.location) > INFECTION_PROXIMITY){
        return false;
    }
    if (try_event(INFECTION_PROBABILITY)) {
        return infect();
    }
    return false;
}

void Person::progress_disease() {
    if (status == INFECTED) {
        disease_counter--;
        if (disease_counter <= 0) {

            // now you either become immune or dead
            // which fatality rate to use
            double fatality_rate = NORMAL_FATALITY_RATE;
            if (saturated) {
                fatality_rate = SATURATED_FATALITY_RATE;
            }
            if (try_event(fatality_rate)) {
                status = DEAD;
            } else {
                status = IMMUNE;
            }
        }
    }
}

bool Person::is_alive() {
    return status != DEAD;
}


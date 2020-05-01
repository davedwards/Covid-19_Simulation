#include "ppmodel.h"
#include "location.h"
#include "simulator.h"
#include "person.h"
#include <bsd/stdlib.h>

static Location popularPlaces[NUM_POPULAR_PLACES];

PopularPlacesModel::PopularPlacesModel() : MobilityModel() {
    speed = -1;
    home = new Location();
    if (try_event(DISTANCING_PROBABILITY)) {
        // I'm distancing
        home_probability = DISTANCING_HOME_PROBABILITY;
    } else {
        // I'm a COVIDiot
        home_probability = NOT_DISTANCING_HOME_PROBABILITY;
    }
}

// why do I compare 'distance' with 'speed'? I have passed a 'speed' variable as a second argument to move_toward() in simulator.cpp.
void PopularPlacesModel::move() {
    if (speed < 0) {
        person->location.setX(home->getX());
        person->location.setY(home->getY());
        pick_new_waypoint();
    } else if (person->location.at_location(*waypoint)){
        stay--;
        if (stay <= 0) {
            pick_new_waypoint();
        }
    } else {
        person->location.move_toward(*waypoint, speed); // 17:10
    }
}

void PopularPlacesModel::pick_new_waypoint() {
    speed = arc4random_uniform(PP_TOP_SPEED+1);
    stay = arc4random_uniform(MAX_STAY);
    if (try_event(home_probability)) {
        waypoint = home;
    } else {
        // pick a random popular place and go there
        int place = arc4random_uniform(NUM_POPULAR_PLACES);
        waypoint = &(popularPlaces[place]);
    }
}

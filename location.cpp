#include "location.h"
#include <math.h>
#include "simulator.h"
#include <bsd/stdlib.h>
// #include "arc4random_uniform.h"

#define CLOSE_ENOUGH 0.05 // location less than this distance apart are considered the same.

Location::Location() {
    x = arc4random_uniform(X_LIMIT);
    y = arc4random_uniform(Y_LIMIT);
}

Location::Location(double nx, double ny){
    x = nx;
    y = ny;
}

bool Location::move_toward(Location destination, double howfar){
    double dx = destination.x-x;
    double dy = destination.y-y;
    double theta = atan2(dy, dx);

    double distance = sqrt((dx*dx) + (dy*dy));

    // 4:04
    // prevents overshooting a point
    // stop at destination while moving towards it, 
    // otherwise we could overshoot
    if (distance < howfar){
        x = destination.x;
        y = destination.y;
        return true;
    } else {
        x = x + (howfar * cos(theta));
        y = y + (howfar * sin(theta));
        return false;
    }
}

double Location::get_distance(Location other_location){
    double dx = other_location.x - x;
    double dy = other_location.y - y; 
    return sqrt((dx*dx) + (dy*dy));
}

bool Location::at_location(Location destination){
    return (get_distance(destination) < CLOSE_ENOUGH);
}

double Location::getX() { return x; }
double Location::getY() { return y; }
void Location::setX(double nx) { x = nx; }
void Location::setY(double ny) { y = ny; }

#ifndef LOCATION_H
#define LOCATION_H

class Location{
private:
    double x, y;
public:
    Location();
    Location(double nx, double ny);

    double getX();
    double getY();
    void setX(double nx);
    void setY(double ny);

    double get_distance(Location other_location);
    bool move_toward(Location destination, double speed);
    bool at_location(Location destination);
};

#endif

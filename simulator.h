// contains general defines for the simulator 

#ifndef SIMULATOR_H
#define SIMULATOR_H


// physical limits to the simulated space
#define X_LIMIT 5000
#define Y_LIMIT 5000

// general simulation parameters
#define SIM_HOURS 5000
#define NUM_PEOPLE 1000

// stagges of the disease
enum disease_status {VULNERABLE, INFECTED, IMMUNE, DEAD};

// how many hours the infection lasts
#define INFECTION_TIME (24*20)

// likelihood of infection when exposed
#define INFECTION_PROBABILITY 0.5

// disease dynamics
#define NORMAL_FATALITY_RATE 0.01   // you can get medical
#define SATURATED_FATALITY_RATE 0.06 // you can't
#define INFECTION_PROXIMITY 5.0
#define INITIAL_INFECTIONS 5

// how many sick people at a time can the health system handle, before saturating.
#define SATURATION_THRESHOLD (NUM_PEOPLE / 5)

// is the system saturated?
extern bool saturated;

// roll the dice, returns true if the event occured
// false otherwise.
bool try_event(double probability);

#endif
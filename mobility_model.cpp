#include <stddef.h>
#include <assert.h>
#include "mobility_model.h"
#include "person.h"

MobilityModel::MobilityModel() {
    person = NULL;
}

void MobilityModel::setPerson(Person* p) {
    assert(p != NULL);
    person = p;
}
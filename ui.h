#ifndef SIMUI_H
#define SIMUI_H

class Person;

typedef int (*sim_func_t)();

int start_ui(sim_func_t simfunc);
void ui_redraw(Person *, int, int*, int);
#endif
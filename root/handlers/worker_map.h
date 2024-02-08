#ifndef ROOT_HANDLERS_WORKER_MAP_H
#define ROOT_HANDLERS_WORKER_MAP_H

#include <map>
#include <iostream>

extern std::map<int, int> worker_map;

void append_worker(int id, int pid);
int count_workers();
bool is_reserved(int id);
int get_pid(int id);
void remove_worker(int id);


#endif // !ROOT_HANDLERS_WORKER_MAP_H

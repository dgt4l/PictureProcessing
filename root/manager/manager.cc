#include "manager.h"


bool ResourseManager::delegate_resources(int amount_, int id) {
            if (balance >= amount_) {
                balance -= amount_;
                set_limit(id, amount_);
                // * worker map associate with id specific amount
            } else {
                // ? no available resources for this task
                return solve_resource_shortage(amount_, id);
            }
            return true;
        }

bool ResourseManager::solve_resource_shortage(int amount_, int id){
    slow_solve_thread();
    return false;
}

void ResourseManager::slow_solve_thread(){
    // * do fucking slow solve
    // * pretty tough to implement but essential
    switch (decide_strategy()) {
    // * ...
        case SURRENDER: {
            // * notify client about failed attempt to allocate resources
            break;
        }
        case DESOLATE: {

            break;
        }
        case DIVIDE: {

            break;
        }
    }
}

// int decide_strategy(){

//     return 1;
// }

void ResourseManager::return_threads(int id) {
    int buff = get_limit(id);
    set_limit(id, 0);
    // * set worker threads to zero
    balance += buff; // * here goes worker balance
}
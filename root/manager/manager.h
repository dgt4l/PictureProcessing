#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <iostream>

class ResourseManager {
    private:
        int resource_amount;
        int min_amount;
    public:
        ResourseManager();

        ~ResourseManager();

        int getResourceValue() const;

        void setResourceValue(int value);

        int calculateResource(int id, int filter);

        
};

#endif

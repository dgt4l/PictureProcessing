#include "manager.h"
#include <iostream>

ResourceManager::ResourceManager() {
    resource_amount = 0;
    min_amount = 0;
    std::cout << "Resource allocated." << std::endl;
}

ResourceManager::~ResourceManager() {
    resource_amount = 0;
    min_amount = 0;
    std::cout << "Resource deallocated." << std::endl;
}

int ResourceManager::getResourceValue() const {
    return resource;
}

void ResourceManager::setResourceValue(int value) {
    resource = value;
}

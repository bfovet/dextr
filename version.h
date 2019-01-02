#ifndef DEXTR_VERSION_H
#define DEXTR_VERSION_H

#include <string>

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 0

const std::string version = std::to_string(VERSION_MAJOR) + "."
        + std::to_string(VERSION_MINOR) + "."
        + std::to_string(VERSION_PATCH);

#endif //DEXTR_VERSION_H

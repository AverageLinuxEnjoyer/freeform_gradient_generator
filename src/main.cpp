#include "application.hpp"

#define FMT_HEADER_ONLY
#include "fmt/core.h"


int main() {

    Application app("Gradient Generator", 1280, 720);
    app.run();
    
    return 0;
}
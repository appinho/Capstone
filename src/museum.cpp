#include "museum.h"
#include <algorithm>

Museum::Museum(){

}

Museum::~Museum()
{
    // set up thread barrier before this object is destroyed
    std::for_each(_threads.begin(), _threads.end(), [](std::thread &t) {
        t.join();
    });
}
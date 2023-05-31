#include <iostream>
#include <thread>
#include <FreeImage.h>
#include "helper_functions.h"

int main() 
{
    std::thread t([](){Say_Hello("Oingo");});
    
    t.join();

    
    return 0;
}
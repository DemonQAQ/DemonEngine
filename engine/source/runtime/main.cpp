//
// Created by Demon on 2023/12/6.
//

#include "base/RuntimeApplication.hpp"
#include <iostream>

using namespace std;

int main()
{
    runtime::RuntimeApplication app;
    if (!app.start())
    {
        std::cerr << "Application failed to start." << std::endl;
        return -1;
    }
    return 0;
}
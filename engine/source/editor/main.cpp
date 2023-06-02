#include <iostream>
#include "interface/Application.hpp"

using namespace runtime;

namespace runtime
{
    extern Application *app;
}

int main(int argc, char **argv)
{
    int ret;

    if ((ret = app->initialize()) != 0)
    {
        printf("init fail");
        return ret;
    }

    while (!app->isQuit())
    {
        app->tick();
    }

    app->finalize();
    return 0;
}

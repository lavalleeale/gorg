// Local Headers
#include <gorg.h>

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];
        if (arg == "--version" || arg == "-v")
        {
            std::cout << "gorg " << GORG_VERSION << std::endl;
            return 0;
        }
    }

    Gorg gorg;
    gorg.setupArguments(argc, argv);
    gorg.setupWindow();
    gorg.setupKeybindings();
    gorg.setupActions();
    gorg.search();
    return gorg.run();
}

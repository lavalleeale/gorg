// Local Headers
#include <gorg.h>

int main(int argc, char *argv[])
{
    Gorg gorg;
    gorg.setupArguments(argc, argv);
    gorg.setupWindow();
    gorg.setupKeybindings();
    gorg.setupActions();
    gorg.search();
    return gorg.run();
}
#include "GameSetupGUI.h"
#include <FL/Fl.H>

int main()
{
    //GameSetupGUI *gui = new GameSetupGUI(5, 10);
    GameSetupGUI *gui = new GameSetupGUI;
    Fl::scheme("gtk+");
    return Fl::run();
}
#include <iostream>
#include "universalFunction/helperFunctions.hpp"
#include "entityManager/master_manager.hpp"
#include "ui/screen_controller.hpp"
#include "AppContext.hpp"

int main() {

    // std::cout << "Ke" << std::endl;
    MasterManager masterManager;
    IOSubManager iOSubManager;
    ScreenController screenController(masterManager, iOSubManager);

    AppContext(masterManager, iOSubManager, screenController).run();

    return 0;
}

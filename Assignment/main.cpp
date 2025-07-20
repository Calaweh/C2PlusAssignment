#include <iostream>
#include "universalFunction/helperFunctions.hpp"
#include "entityManager/master_manager.hpp"
#include "ui/screen_controller.hpp"

int main() {

    std::cout << "Ke" << std::endl;
    MasterManager masterManager;
    ScreenController screenController(masterManager);

    

    return 0;
}

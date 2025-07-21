#pragma once
#include "universalFunction/helperFunctions.hpp"
#include "ui/screen_controller.hpp"
#include "entityManager/master_manager.hpp"

struct AppContext
{
private:
    MasterManager &masterManager;
    IOSubManager &iOSubManager;
    ScreenController &screenController;

public:
    AppContext(MasterManager &masterManager,
               IOSubManager &iOSubManager,
               ScreenController &screenController)
        : masterManager(masterManager),
          iOSubManager(iOSubManager),
          screenController(screenController) {}

    const MasterManager &getMasterManager() const { return masterManager; }
    IOSubManager &getIOSubManager() const { return iOSubManager; }
    const ScreenController &getScreenController() const { return screenController; }

    void run()
    {
        screenController.navigateToLogin();
    }

};
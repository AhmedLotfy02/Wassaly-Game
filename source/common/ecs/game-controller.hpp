#pragma once

#include "component.hpp"
#include "transform.hpp"
#include <list>
#include <iterator>
#include <string>
#include <glm/glm.hpp>
#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>

#include "../application.hpp"
namespace our
{

    class GameController
    {
        Application *app; // The application in which the state runs
        our::World world;
        our::FreeCameraControllerSystem cameraController;

        int numberOfBatteries = 5;
        int counterToRemove = 0;
        int tempCount = 0;

    public:
        GameController(Application *app)
        {
            this->app = app;
                }
        void increaseBatteries()
        {
            if (numberOfBatteries == 4)
            {
                numberOfBatteries = 5;

                world.markAsUnRemoval("e5");
                cameraController.changeSpeed(0.7f);
            }
            else if (numberOfBatteries == 3)
            {
                numberOfBatteries = 4;
                world.markAsUnRemoval("e4");
                cameraController.changeSpeed(0.6f);
            }
            else if (numberOfBatteries == 2)
            {
                numberOfBatteries = 3;
                world.markAsUnRemoval("e3");
                cameraController.changeSpeed(0.5f);
            }
            else if (numberOfBatteries == 1)
            {
                numberOfBatteries = 2;
                world.markAsUnRemoval("e3");
                cameraController.changeSpeed(0.4f);
            }
        }

        void decreaseBatteries()
        {
            if (counterToRemove == -180)
            {
                counterToRemove = 0;
                if (numberOfBatteries == 5)
                {
                    numberOfBatteries = 4;
                    world.removeBatteryEntity("e5");
                    cameraController.changeSpeed(0.6f);
                }
                else if (numberOfBatteries == 4)
                {
                    numberOfBatteries = 3;
                    world.removeBatteryEntity("e4");
                    cameraController.changeSpeed(0.5f);
                }
                else if (numberOfBatteries == 3)
                {
                    numberOfBatteries = 2;
                    world.removeBatteryEntity("e3");
                    cameraController.changeSpeed(0.4f);
                }
                else if (numberOfBatteries == 2)
                {
                    numberOfBatteries = 1;
                    world.removeBatteryEntity("e2");
                    cameraController.changeSpeed(0.3f);
                }
                else
                {
                    numberOfBatteries = 0;
                    world.removeBatteryEntity("e1");
                    getApp()->changeState("gameover");
                    /// Game over
                }
            }
        }
    };

}
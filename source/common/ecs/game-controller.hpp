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
#include "../ecs/world.hpp"
#include "../systems/free-camera-controller.hpp"
#include "../application.hpp"
namespace our
{

    class GameController
    {
        Application *app; // The application in which the state runs
        World *world;
        // Free////cameraControllerSystem //cameraController;
        int numberOfBatteries = 5;
        int counterToRemove = 0;
        int tempCount = 0;

    public:
        GameController()
        {
        }
        void enter(Application *app, World *world)
        {
            this->app = app;
            // this->//cameraController = //cameraController;
            this->world = world;
        }
        void increaseBatteries(int *numberOfBatteries)
        {
            if (*numberOfBatteries == 4)
            {
                *numberOfBatteries = 5;

                world->markAsUnRemoval("e5");
                // cameraController.changeSpeed(0.7f);
            }
            else if (*numberOfBatteries == 3)
            {
                *numberOfBatteries = 4;
                world->markAsUnRemoval("e4");
                // cameraController.changeSpeed(0.6f);
            }
            else if (*numberOfBatteries == 2)
            {
                *numberOfBatteries = 3;
                world->markAsUnRemoval("e3");
                // cameraController.changeSpeed(0.5f);
            }
            else if (*numberOfBatteries == 1)
            {
                *numberOfBatteries = 2;
                world->markAsUnRemoval("e3");
                // cameraController.changeSpeed(0.4f);
            }
        }

        float decreaseBatteries(int *counterToRemove, int *numberOfBatteries, bool remove)
        {
            // std::cout << counterToRemove;
            if (*counterToRemove == 1180 || remove)
            {
                *counterToRemove = 0;
                if (*numberOfBatteries == 5)
                {
                    *numberOfBatteries = 4;
                    world->removeBatteryEntity("e5");
                    // cameraController.changeSpeed(0.6f);
                    return 0.6f;
                }
                else if (*numberOfBatteries == 4)
                {
                    *numberOfBatteries = 3;
                    world->removeBatteryEntity("e4");
                    // cameraController.changeSpeed(0.5f);
                    return 0.5f;
                }
                else if (*numberOfBatteries == 3)
                {
                    *numberOfBatteries = 2;
                    world->removeBatteryEntity("e3");
                    // cameraController.changeSpeed(0.4f);
                    return 0.4f;
                }
                else if (*numberOfBatteries == 2)
                {
                    *numberOfBatteries = 1;
                    world->removeBatteryEntity("e2");
                    // cameraController.changeSpeed(0.3f);
                    return 0.3f;
                }
                else
                {
                    *numberOfBatteries = 0;
                    world->removeBatteryEntity("e1");
                    app->changeState("gameover");
                    return 0.0f;

                    /// Game over
                }
                return 0.7f;
            }
            return 0.7f;
        }
    };

}
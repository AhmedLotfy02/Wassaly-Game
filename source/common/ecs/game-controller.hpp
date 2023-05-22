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
        float lastSpeed=3.0f;

    public:
        GameController()
        {
        }
        void enter(Application *app, World *world)
        {
            this->app = app;
            this->world = world;
            this->lastSpeed=3.0f;
            
        }
        int increaseBatteries(int *numberOfBatteries)
        {
           
       
            if (*numberOfBatteries == 4)
            {

                *numberOfBatteries = 5;

                world->markAsUnRemoval("plane5");

                
                this->lastSpeed=3.0f;
                return 3.0f;
            }
            else if (*numberOfBatteries == 3)
            {
                *numberOfBatteries = 4;
                world->markAsUnRemoval("plane4");
                this->lastSpeed=2.5f;
                return 2.5f;
                
            }
            else if (*numberOfBatteries == 2)
            {
                *numberOfBatteries = 3;
                world->markAsUnRemoval("plane3");
                this->lastSpeed=2.0f;
                return 2.0f;
               
            }
            else if (*numberOfBatteries == 1)
            {
                *numberOfBatteries = 2;
                world->markAsUnRemoval("plane2");
                this->lastSpeed=1.7f;
                return 1.7f;
            }
        }

        float decreaseBatteries(int *counterToRemove, int *numberOfBatteries, bool remove)
        {
           
            if (*counterToRemove == -180 || remove)
            {
                std::cout << "decrease" << std::endl;
                *counterToRemove = 0;
                if (*numberOfBatteries == 5)
                {
                    *numberOfBatteries = 4;
                    world->removeBatteryEntity("plane5");
                  
                    lastSpeed=2.5f;
                    return 2.5f;
                }
                else if (*numberOfBatteries == 4)
                {
                    *numberOfBatteries = 3;
                    world->removeBatteryEntity("plane4");
                  
                    lastSpeed=2.0f;
                    return 2.0f;
                }
                else if (*numberOfBatteries == 3)
                {
                    *numberOfBatteries = 2;
                    world->removeBatteryEntity("plane3");
                   
                    lastSpeed=1.7f;
                    return 1.7f;
                }
                else if (*numberOfBatteries == 2)
                {
                    *numberOfBatteries = 1;
                    world->removeBatteryEntity("plane2");
                   
                    lastSpeed=1.5f;
                    return 1.5f;
                }
                else
                {
                    /// Game over
                    *numberOfBatteries = 0;
                    world->removeBatteryEntity("plane1");
                    app->changeState("gameover");
                    return 0.0f;

                }
               
            }
            return lastSpeed;
        }
    };

}
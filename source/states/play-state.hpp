#pragma once
#include <iostream>
#include <application.hpp>
#include <systems/collision.hpp>
#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include "../common/ecs/game-controller.hpp"
// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CollisionSystem collisionSystem;
    our::GameController gameController;

    int numberOfBatteries = 5;
    int counterToRemove = 0;
    int tempCount = 0;
    int packagesNumber = 0;

    void onInitialize() override
    {
        numberOfBatteries = 5;
        counterToRemove = 0;
        tempCount = 0;
        gameController.enter(getApp(), &world);
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp(), &gameController);
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override
    {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime, &counterToRemove, &numberOfBatteries);
        counterToRemove++;

        float speed = gameController.decreaseBatteries(&counterToRemove, &numberOfBatteries, false);
        cameraController.changeSpeed(speed);
        //    decreaseBatteries();

        // tempCount++;
        //   if(tempCount==400){
        //     tempCount=0;
        //     increaseBatteries();
        // }
        // And finally we use the renderer system to draw the scene

        // Collioison

        int collisionState = collisionSystem.update(&world, (float)deltaTime);
        // std::cout << "collisionState" << collisionState << std::endl;
        if (collisionState == 1)
        {
            std::cout << "collision with battery" << std::endl;
            gameController.increaseBatteries(&numberOfBatteries);
        }
        else if (collisionState == -1)
        {
            std::cout << "gameover" << std::endl;
            getApp()->changeState("gameover");
        }
        else if (collisionState == 2)
        {
            // TODO : add score for package
            std::cout << "collision with package" << std::endl;
        }

        renderer.render(&world);

        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_ESCAPE))
        {
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override
    {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};
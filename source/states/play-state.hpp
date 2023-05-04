#pragma once
#include <iostream>
#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    int numberOfBatteries=5;
    int counterToRemove=0;
    int tempCount=0;

    void increaseBatteries(){
        if(numberOfBatteries==4){
                numberOfBatteries=5;
              
               world.markAsUnRemoval("e5");
                cameraController.changeSpeed(0.7f);
            }
            else if(numberOfBatteries==3)
            {
                numberOfBatteries=4;
                world.markAsUnRemoval("e4");
                 cameraController.changeSpeed(0.6f);
            }
                        else if(numberOfBatteries==2)
            {
                numberOfBatteries=3;
                world.markAsUnRemoval("e3");
                 cameraController.changeSpeed(0.5f);
            }
             else if(numberOfBatteries==1)
            {
                numberOfBatteries=2;
                world.markAsUnRemoval("e3");
                 cameraController.changeSpeed(0.4f);
            }
           

    }
    void decreaseBatteries(){
        if(counterToRemove==180){
            counterToRemove=0;
            if(numberOfBatteries==5){
                numberOfBatteries=4;
                world.removeBatteryEntity("e5");
                cameraController.changeSpeed(0.6f);
            }
            else if(numberOfBatteries==4)
            {
                numberOfBatteries=3;
                world.removeBatteryEntity("e4");
                 cameraController.changeSpeed(0.5f);
            }
                        else if(numberOfBatteries==3)
            {
                numberOfBatteries=2;
                world.removeBatteryEntity("e3");
                 cameraController.changeSpeed(0.4f);
            }
             else if(numberOfBatteries==2)
            {
                numberOfBatteries=1;
                world.removeBatteryEntity("e2");
                 cameraController.changeSpeed(0.3f);
            }
             else 
            {
                numberOfBatteries=0;
                world.removeBatteryEntity("e1");
                 getApp()->changeState("gameover");
                /// Game over
            }
        }
    }
    void onInitialize() override {
         numberOfBatteries=5;
         counterToRemove=0;
        tempCount=0;
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        // tempCount++;
        //   if(tempCount==400){
        //     tempCount=0;
        //     increaseBatteries();
        // }
        // And finally we use the renderer system to draw the scene
        counterToRemove++;
        decreaseBatteries();
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto& keyboard = getApp()->getKeyboard();

        if(keyboard.justPressed(GLFW_KEY_ESCAPE)){
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override {
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
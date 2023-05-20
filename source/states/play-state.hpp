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
#include "../common/ecs/entity.hpp"

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
    float time = 0;
    bool effect = false;
    float lastCollisionTimeBattery = 0;
    float lastCollisionTimePackage = 0;
    bool won=false;
    float speed=0;
    bool end=false;

    void onInitialize() override
    {
        numberOfBatteries = 5;
        counterToRemove = 0;
        tempCount = 0;
        packagesNumber = 0;
        lastCollisionTimeBattery = 0;
        lastCollisionTimePackage = 0;
        won=false;
        speed=0;
        gameController.enter(getApp(), &world);
        end=false;
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
        
        our::Entity* player = world.getEntityByName("player");
        collisionSystem.setPlayer(player);
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp(), &gameController);
        cameraController.changeSpeed(3.0f);
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }
    void onImmediateGui() override
    {
        // We use the immediate GUI to draw the debug information of the renderer
       ImVec2 packagePos(790, 80);

        ImGui::SetNextWindowPos(packagePos);
        ImGui::SetNextWindowSize({260, 30});
        ImGui::Begin("Package", NULL,
                         ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoSavedSettings |
                         ImGuiWindowFlags_NoInputs |
                         ImGuiWindowFlags_AlwaysAutoResize);
        // blue color
        ImVec4 packageColor(0.0f, 0.0f, 1.0f, 1.0f);
        ImGui::TextColored(packageColor, "Packages: %d /4", packagesNumber);
        ImGui::SetWindowFontScale(2.0f);
        
        ImGui::End();
    }
    void onDraw(double deltaTime) override
    {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime, &counterToRemove, &numberOfBatteries, &won,&end);
        counterToRemove++;

        speed = gameController.decreaseBatteries(&counterToRemove, &numberOfBatteries, false);
        std::cout<<speed<<std::endl;
        cameraController.changeSpeed(speed);
        //    decreaseBatteries();

        // tempCount++;
        //   if(tempCount==400){
        //     tempCount=0;
        //     increaseBatteries();
        // }
        // And finally we use the renderer system to draw the scene

        // Collioison
        if(end){
            if(packagesNumber>=4){
                 getApp()->changeState("youwon");
            }
            else{
                    getApp()->changeState("gameover");
            }
        }
        // if(won){
        //     getApp()->changeState("youwon");
        // }
        
        int collisionState = collisionSystem.update(&world, (float)deltaTime);
       
           
            if (collisionState == 1 && glfwGetTime()-lastCollisionTimeBattery>2.0f)
            { 
                
                lastCollisionTimeBattery = glfwGetTime();
                std::cout << "collision with battery" << std::endl;
                effect = true;
                time = glfwGetTime();
               speed= gameController.increaseBatteries(&numberOfBatteries);
               counterToRemove=0;
               
            }
            else if (collisionState == -1)
            {
                std::cout << "gameover" << std::endl;
                getApp()->changeState("gameover");
            }
            else if (collisionState == 2 && glfwGetTime()-lastCollisionTimePackage>2.0f)
            {
                  std::cout<<"inside"<<std::endl;
                lastCollisionTimePackage = glfwGetTime();
                packagesNumber++;
                // if(packagesNumber==4)
                // {
                //     std::cout << "youwon" << std::endl;
                //     getApp()->changeState("youwon");
                // }
                // TODO : add score for package
            // std::cout << "collision with package" << std::endl;
            }
        
        
        
        if( effect && glfwGetTime()- time > 2.0f)
        {
           // std::cout << "effect1111111111" << effect << std::endl;
            effect = false;
        }
        
        //std::cout << "effect" << effect << std::endl;
        renderer.render(&world, effect);

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
#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../common/ecs/game-controller.hpp"

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
        float speed = 7.0f;
        our::GameController *gameController;
        bool decreaseBat = false;
        int powers=3;
        float lastPowered=0.0f;
        bool powered=false;
        float lastBuildingCollision=0.0f;
        bool donotrepeatpower=true;

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app, GameController *gameCon)
        {
            this->app = app;
            this->gameController = gameCon;
            this->speed=3.5f;
            this->powers=3;
        }
        void changeSpeed(float speed)
        {
            this->speed = speed;
           
        }
        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime, int *countToRemove, int *numOfBatteries,bool* won,bool* end,bool* effect2,bool* effect3,int* powerUPs)
        {
            
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = camera->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation

            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            {
                glm::vec2 delta = app->getMouse().getMouseDelta();
                rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f)
                rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f)
                rotation.x = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor

            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT) &&powers>0&&donotrepeatpower){
                
                powers--;
                *powerUPs=*powerUPs-1;
                lastPowered=glfwGetTime();
                 powered=true;
                 *effect3=true;
                 donotrepeatpower=false;
              
            }
            if(powered){
               
                current_sensitivity *= controller->speedupFactor;
                
            }
            if(glfwGetTime()-lastPowered>0.5f){
                powered=false;
                *effect3=false;
                donotrepeatpower=true;
            }
            

            if(glfwGetTime()-lastBuildingCollision>2.0f){
                *effect2=false;
            }
            // AL: going forward
            current_sensitivity *= speed;
            position += front * (deltaTime * current_sensitivity.z);
            if ((position.x > 5.5 && position.x < 5.7) && decreaseBat)
            {
                decreaseBat = false;
            }
          
            else if (position.x > 6.1 && position.x < 6.14 && !decreaseBat)
            {
              
              lastBuildingCollision=glfwGetTime();
                *effect2=true;
                 gameController->decreaseBatteries(countToRemove, numOfBatteries, true);
                decreaseBat = true;
            }

            if ((position.x < -6.5 && position.x > -7.7) && decreaseBat)
            {
              
                decreaseBat = false;
            }

            else if (position.x > -8.09 && position.x < -7.9 && !decreaseBat)
            {
                 lastBuildingCollision=glfwGetTime();
                  *effect2=true;
               
                gameController->decreaseBatteries(countToRemove, numOfBatteries, true);
                decreaseBat = true;
            }
             if(position.z<-490.0){
                    *won=true;
                    *end=true;
            }
            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
           
               
            if (app->getKeyboard().isPressed(GLFW_KEY_S))
                position -= front * (deltaTime * current_sensitivity.z);
          
            if (app->getKeyboard().isPressed(GLFW_KEY_D))
            {
                if (!(position.x > 6.1))
                    position += right * (deltaTime * current_sensitivity.x);
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_A))
            {
               
                if (!(position.x < -8))
                    position -= right * (deltaTime * current_sensitivity.x);
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };

}

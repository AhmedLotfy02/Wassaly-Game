#pragma once
#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/mesh-renderer.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <string>

namespace our
{

    class CollisionSystem
    {
        //std::vector<Entity *> cars, batteries, planes, packages;
        Entity *player;

    public:
        void setPlayer(Entity *player)
        {
            this->player = player;
        }
        float two_d_collides(const glm::vec4 &carMax, const glm::vec4 &carMin, const glm::vec4 &batteryMax, const glm::vec4 &batteryMin)
        {
            // calcculate the eclidian distance between the centers of the two objects
            float distance = glm::distance(glm::vec3(carMax.x, carMax.y, carMax.z), glm::vec3(batteryMax.x, batteryMax.y, batteryMax.z));
            return distance;
            
            /*
            // apply the following algorithm a.minX <= b.maxX && a.maxX >= b.minX && a.minY <= b.maxY && a.maxY >= b.minY && a.minZ <= b.maxZ && a.maxZ >= b.minZ
            if ( (carMax.x >= batteryMin.x && carMin.x <= batteryMax.x &&
                    carMax.y >= batteryMin.y && carMin.y <= batteryMax.y &&
                    carMax.z >= batteryMin.z && carMin.z <= batteryMax.z) )
            {
                std::cout<< "collision happened" << std::endl;
                return true;
            }
            else
            {
                //std::cout<< "collision didn't happen" << std::endl;
                return false;
            }
            */
        }

        bool checkCollision(Entity *batteryComponent, Entity *carComponent)
        {

            // get centers and difference between centers
            glm::vec4 carCenter = carComponent->getLocalToWorldMatrix() * glm::vec4(carComponent->localTransform.position, 1.0);
            glm::vec4 batteryCenter = glm::vec4(batteryComponent->localTransform.position, 1.0);

            glm::vec4 carMax = carCenter + glm::vec4(carComponent->localTransform.scale, 0.0);
            glm::vec4 carMin = carCenter - glm::vec4(carComponent->localTransform.scale, 0.0);

            glm::vec4 batteryMax = batteryCenter + glm::vec4(batteryComponent->localTransform.scale, 0.0);
            glm::vec4 batteryMin = batteryCenter - glm::vec4(batteryComponent->localTransform.scale, 0.0);

            // check two_d_collides for x and z
            bool result = false;
            result = (two_d_collides(carMax, carMin, batteryMax, batteryMin) < 2.5f) ? 1 : 0;
            return result;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        int update(World *world, float deltaTime)
        {
           
             
            // iterate over all entities in the world and check for collisions with the player
            for (const auto &entity : world->getEntities())
            {
                

               
                if(checkCollision(entity, player)&&entity->name!="player")
                {
                    std::cout<<"collision happened"<<std::endl;
                    if(entity->name.substr(0, 7) == "battery")
                    {
                        world->markForRemoval(entity);
                        return 1;
                    }
                    else if ((entity->name.substr(0, 2) == "cr") || (entity->name.substr(0, 2) == "cl"))
                    {
                        world->markForRemoval(entity);
                        return -1;
                    }
                    else if ( (entity->name.substr(0, 7) =="package") )
                    {
                        world->markForRemoval(entity);
                        return 2;
                    }
                    return 1;
                }


               
            }


            return 0;
        }
    };
}
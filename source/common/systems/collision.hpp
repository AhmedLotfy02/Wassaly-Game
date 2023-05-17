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
        std::vector<Entity *> cars, batteries;
        std::vector<MeshRendererComponent *> healthBar;
        Entity *player;

    public:
        // AABB Axis Aligned Bounding Box
        // AABB - Circle collision [battery-AABB car-circle]
        // return pair of two flags
        // first flag indicates whether the collision happened [1-->collision happened, 0-->no collision]
        // second flag indicates whether the collision happened with a battery that will increase the health
        // or a battery that will decrease the health [1-->positive battery, 0-->negative battery]
        
        float ecludianDistance(glm::vec3 a, glm::vec3 b)
        {
            return sqrt(pow(a.x - b.x, 2) + pow(a.z - b.z, 2));
        }

        bool checkCollision(Entity *batteryComponent, Entity *carComponent , float threshold)
        {
            //std::cout<< "ecludianDistance" << std::endl;  
            
            // get centers and difference between centers
            glm::vec3 carCenter = carComponent->localTransform.position;
           // std::cout<< "ecludianDistance" << std::endl;  
            
            glm::vec3 batteryCenter = batteryComponent->localTransform.position;
            //std::cout<< "ecludianDistance" << std::endl;  
            
            float distance = ecludianDistance(carCenter, batteryCenter);
            //std::cout<< "ecludianDistance" << std::endl;  
            // try and error
            bool result = false;
            result = (distance < threshold) ? 1 : 0;
            //std::cout<< "ecludianDistance" << std::endl;
            return result;
        }


        // sort based on x of position
        std::vector<MeshRendererComponent *> sortHealthBars(std::vector<MeshRendererComponent *> vec)
        {

            std::sort(vec.begin(), vec.end(), [](MeshRendererComponent *a, MeshRendererComponent *b)
                      { return a->getOwner()->localTransform.position.x < b->getOwner()->localTransform.position.x; });
            return vec;
        }

        int update(World *world, float deltaTime )
        {
            batteries.clear();
            cars.clear();
            healthBar.clear();
            
            //std::cout<<"I am insise updaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaate";
            for (const auto &entity : world->getEntities())
            {
                //std::cout<<"I am insise foooooooooooooooooooooor";
                //std::cout<<entity->name<<std::endl;
                //std::cout<<entity->name<<std::endl;
                if (entity->name == "battery")
                {
                    //std::cout<<entity->name<<std::endl;
                    batteries.push_back(entity);
                    //std::cout<< entity->localTransform.position.x << std::endl;
                }
                else if (entity->name.find("car"))
                {
                    //std::cout<<"I am caaaaaaaaaaaaaaaaaaaaaaaaaaaaaaar"<<std::endl;
                    cars.push_back(entity);
                }
                else if( entity->name.find("player") )
                {
                    //std::cout<<"I am plaaaaaaaaaaaaaaaaaaaaaaaaayer"<<std::endl;
                    player = entity;
                }
            }
            
            // to sort the healthBars due to their x position
            //std::cout<< "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;
            healthBar = sortHealthBars(healthBar);
            //std::cout<< "bbbbbbbbbbbbbbbbbbbbbbbbbbbbb" << std::endl;
            for (Entity* battery : batteries)
            {
                //std::cout<< battery->localTransform.position.x << std::endl;
                //std::cout<< battery->localTransform.position.y << std::endl;
                //std::cout<< battery->localTransform.position.z << std::endl;
                //std::cout<< "eeeeeeeeeeeeeeeeeeeeeeeee" << std::endl;
                
                std::cout<< "battery" << std::endl;
                
                std::cout<< battery->localTransform.position.x << std::endl;
                std::cout<< battery->localTransform.position.y << std::endl;
                std::cout<< battery->localTransform.position.z << std::endl;
                
                std::cout<< "car" << std::endl;
                
                std::cout<< player->localTransform.position.x << std::endl;
                std::cout<< player->localTransform.position.y << std::endl;
                std::cout<< player->localTransform.position.z << std::endl;
                
                // if collision happened with a battery
                if ( checkCollision(battery, player , 25.0) )
                {
                    //increaseBatteries();
                    //   remove battery after collision
                    std::cout<< "checkCollisionnnnnnnnnnnnnnnnnnnnnnnnnnnnn" << std::endl;
                    world->markForRemoval(battery);
                    world->deleteMarkedEntities();

                    return 1; // collision with battery
                }   
                else
                    std::cout<< "Outside checkCollision" << std::endl;      
            }

            for ( int i=0 ; i<cars.size() ; ++i)
            {
                // if collision happened with a battery
                if (checkCollision(cars[i], player , 5.0))
                {
                    // collision of 2 cars ==> game over
                    //getApp()->changeState("gameover");
                    return -1; //collision with car
                }                
            }

            return healthBar.size();
        }


        void addHealthBar(World *world)
        {
            // add last health bar after collision with battery
            Entity *addedHealthBar = world->add();
            addedHealthBar->parent = healthBar[healthBar.size() - 1]->getOwner()->parent;
            addedHealthBar->name = "healthbar";
            addedHealthBar->addComponent<MeshRendererComponent>();
            addedHealthBar->getComponent<MeshRendererComponent>()->mesh = healthBar[healthBar.size() - 1]->mesh;
            addedHealthBar->getComponent<MeshRendererComponent>()->material = healthBar[healthBar.size() - 1]->material;
            addedHealthBar->localTransform = healthBar[healthBar.size() - 1]->getOwner()->localTransform;
            addedHealthBar->localTransform.scale = {0.06, 0.04, 0.1};
            addedHealthBar->localTransform.position.x = healthBar[healthBar.size() - 1]->getOwner()->localTransform.position.x + 0.15;
            healthBar.push_back(addedHealthBar->getComponent<MeshRendererComponent>());
        }
        
        void removeHealthBar(World *world)
        {
            // remove last health bar after collision
            world->markForRemoval(healthBar[healthBar.size() - 1]->getOwner());
            world->deleteMarkedEntities();
            healthBar.pop_back();
        }
    };
}
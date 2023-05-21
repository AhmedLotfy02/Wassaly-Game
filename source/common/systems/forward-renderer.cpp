#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
#include "components/light.hpp"

namespace our
{
    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json &config)
    {
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if (config.contains("sky"))
        {
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));

            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram *skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();

            // TODO: (Req 10) Pick the correct pipeline state to draw the sky
            //  Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            //  We will draw the sphere from the inside, so what options should we pick for the face culling.
            
            PipelineState skyPipelineState{};
            skyPipelineState.depthTesting.enabled  = true;
            skyPipelineState.depthTesting.function = GL_LEQUAL;

            skyPipelineState.faceCulling.enabled    = true;
            skyPipelineState.faceCulling.culledFace = GL_FRONT;

            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D *skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky
            Sampler *skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if (config.contains("postprocess"))
        {
            // TODO: (Req 11) Create a framebuffer
            //  ==> create a framebuffer
            //  parameters:
            //   ==> 1. the number of framebuffers to be generated
            //   ==> 2. the address of the framebuffer object names to be generated
            glGenFramebuffers(1, &postprocessFrameBuffer);

            // ==> bind the framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);

            // TODO: (Req 11) Create a color and a depth texture and attach them to the framebuffer
            //  Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            //  The depth format can be (Depth component with 24 bits).

            // ==> create a color texture with the format of RGBA8
            colorTarget = texture_utils::empty(GL_RGBA8, windowSize);

            // ==> create a depth texture with the format of DEPTH_COMPONENT24
            depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT24, windowSize);

            // ==> attach the color texture to the framebuffer
            // parameters:
            //  ==> 1. the target of the framebuffer ==> GL_FRAMEBUFFER
            //  ==> 2. the attachment point ==> GL_COLOR_ATTACHMENT
            //  ==> 3. the texture target ==> 2D texture
            //  ==> 4. the texture object name
            //  ==> 5. the mipmap level of the texture ==> 0 means the base level
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);

            // ==> attach the depth texture to the framebuffer
            // parameters:
            //  ==> 1. the target of the framebuffer ==> GL_FRAMEBUFFER
            //  ==> 2. the attachment point ==> GL_DEPTH_ATTACHMENT0
            //  ==> 3. the texture target ==> 2D texture
            //  ==> 4. the texture object name
            //  ==> 5. the mipmap level of the texture ==> 0 means the base level
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);

            // TODO: (Req 11) Unbind the framebuffer just to be safe
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler *postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram *postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy()
    {
        // Delete all objects related to the sky
        if (skyMaterial)
        {
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if (postprocessMaterial)
        {
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World *world, bool effect,bool effect2,bool effect3)
    {
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent *camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();

        // for lights
        std::vector<LightComponent *> lights;
        for (auto entity : world->getEntities())
        {
            // If we hadn't found a camera yet, we look for a camera in this entity
            if (!camera)
                camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if (auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer)
            {
                if (!(world->checkIfMarkedRemoval(entity)))
                {
                    // We construct a command from it
                    RenderCommand command;
                    command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                    command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                    command.mesh = meshRenderer->mesh;
                    command.material = meshRenderer->material;
                    // if it is transparent, we add it to the transparent commands list
                    if (command.material->transparent)
                    {
                        transparentCommands.push_back(command);
                    }
                    else
                    {
                        // Otherwise, we add it to the opaque command list
                        opaqueCommands.push_back(command);
                    }
                }
            }

            auto light = entity->getComponent<LightComponent>();
            if (light && light->enabled)
            {
                if (light->typeLight == LightType::SKY)
                {
                    auto litShader = AssetLoader<ShaderProgram>::get("light");
                    litShader->use();
                    litShader->set("sky.top", light->sky_light.top_color);
                    litShader->set("sky.middle", light->sky_light.middle_color);
                    litShader->set("sky.bottom", light->sky_light.bottom_color);
                }
                else
                    lights.push_back(light);
            }

        }

        // If there is no camera, we return (we cannot render without a camera)
        if (camera == nullptr)
            return;

        // TODO: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        //  HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        glm::vec3 cameraForward = camera->getViewMatrix()[2];
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand &first, const RenderCommand &second)
                  {
                      // TODO: (Req 9) Finish this function
                      // dot product
                      float firstDistance = glm::dot(first.center, cameraForward);
                      float secondDistance = glm::dot(second.center, cameraForward);
                      return firstDistance < secondDistance;

                      /******************************************************************************************************
                        -- Function parameters:
                            1,2) transparentCommands.begin() and transparentCommands.end() are the iterators that define the range of elements to be sorted.
                                 transparentCommands is a vector of RenderCommand objects that need to be sorted.
                            3) a lambda function that compares two RenderCommand objects based on their distance from the camera's forward vector.
                               The lambda function takes two RenderCommand objects as input and returns a boolean value indicating whether the first object is less than the second object.

                        -- It calculates the distance of the center of each object from the camera's forward vector using the dot product of the center vector
                           and the camera's forward vector. The object with the smaller distance is considered "less than" the object with the larger distance
                           and will be placed before it in the sorted range.
                       ******************************************************************************************************/
                  });

        // TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP
        glm::mat4 VP = camera->getProjectionMatrix(windowSize) * camera->getViewMatrix();

        // TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
        glViewport(0, 0, windowSize.x, windowSize.y);
        /***************************************************************************************************************
          'glViewport' is a function that sets the viewport dimensions of the rendering window.

                      });

        // TODO: (Req 9) Set the clear color to black and the clear depth to 1
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        /***************************************************************************************************************
          'glClearColor' is a function that specifys the clear color for the color buffer.
          -- The function takes four parameters: which represent the red, green, blue, and alpha.
         ***************************************************************************************************************/


            //TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
            glViewport(0, 0, windowSize.x, windowSize.y);
            /***************************************************************************************************************
              'glViewport' is a function that sets the viewport dimensions of the rendering window.

              -- The function takes four parameters:
                 1) '0' an integer value that specifies the x-coordinate of the lower-left corner of the viewport, in window coordinates.
                        In this case, the viewport starts at the left edge of the window.
                 2) '0' an integer value that specifies the y-coordinate of the lower-left corner of the viewport, in window coordinates.
                        In this case, the value is set to 0, which means that the viewport starts at the bottom edge of the window.
                 3) 'windowSize.x' an integer value that specifies the width of the viewport in pixels.
                 4) 'windowSize.x' an integer value that specifies the height of the viewport in pixels.
             ***************************************************************************************************************/

            //TODO: (Req 9) Set the clear color to black and the clear depth to 1
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            /***************************************************************************************************************
              'glClearColor' is a function that specifys the clear color for the color buffer.
              -- The function takes four parameters: which represent the red, green, blue, and alpha.
             ***************************************************************************************************************/

        // If there is a postprocess material, bind the framebuffer
        if (postprocessMaterial)
        {
            // TODO: (Req 11) bind the framebuffer
            //  bind the postprocess framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);
        }

        // TODO: (Req 9) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /***************************************************************************************************************
          'glClear' is a function that clears the specified buffers to their default values.

              -- The function takes four parameters, each representing a boolean value that determines whether writing
                 to the corresponding color component is enabled (GL_TRUE) or disabled (GL_FALSE).
                 The four parameters correspond to the red, green, blue, and alpha components of the color buffer, respectively.
             ***************************************************************************************************************/

            glDepthMask(GL_TRUE);
            /***************************************************************************************************************
              'glDepthMask' is a function that enables or disables writing to the depth buffer.
              -- The function takes a single parameter, which represents a boolean value that determines whether writing
                  to the depth buffer is enabled (GL_TRUE) or disabled (GL_FALSE).
             ***************************************************************************************************************/

            // If there is a postprocess material, bind the framebuffer
            if (postprocessMaterial) {
                //TODO: (Req 11) bind the framebuffer
                // bind the postprocess framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);
            }

            //TODO: (Req 9) Clear the color and depth buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            /***************************************************************************************************************
              'glClear' is a function that clears the specified buffers to their default values.

              -- The function takes a single parameter, which is a bitwise OR combination of one or more constants that represent the buffers to be cleared.
                 In this case, both the color buffer and the depth buffer should be cleared.
             ***************************************************************************************************************/

            glEnable(GL_DEPTH_TEST);
            /***************************************************************************************************************
             'glEnable' is a function that enables various features and capabilities in the graphics pipeline.

             -- The function takes a single parameter, which is a constant that represents the feature or capability to be enabled.
                In this case, GL_DEPTH_TEST enables depth testing in the graphics pipeline.

             -- Depth testing is a technique used to ensure that closer objects are rendered in front of farther objects,
                based on their distance from the viewer's perspective. This is typically achieved by comparing the depth values of pixels
                 in the depth buffer during the rendering process.
            ***************************************************************************************************************/

            //TODO: (Req 9) Draw all the opaque commands
            // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command

        int numLights = lights.size();
        //    for(auto command : opaqueCommands){
        //     command.material->setup();
        //     command.material->shader->set("transform", VP*command.localToWorld);
        //     command.mesh->draw();
        // }
        int h=0;
        for(auto opaqueCommand: this->opaqueCommands) {
            opaqueCommand.material->setup();

            //set the camera position
            if (dynamic_cast<our::LightMaterial *>(opaqueCommand.material)) {

               // std::cout << "dynamic cast\n";
                opaqueCommand.material->shader->set("camera_position", cameraForward);
                //set the M matrix for the shader
                opaqueCommand.material->shader->set("object_to_world", opaqueCommand.localToWorld);
                //set the M_IT matrix for the shader
                opaqueCommand.material->shader->set("object_to_world_inv_transpose", glm::transpose(glm::inverse(opaqueCommand.localToWorld)));
                //set the VP matrix for the shader
                opaqueCommand.material->shader->set("view_projection", VP);
            }
            else opaqueCommand.material->shader->set("transform", VP * opaqueCommand.localToWorld);
            const int MAX_LIGHT_COUNT = 8;

            opaqueCommand.material->shader->set("light_count", numLights);

            int light_index = 0;
            for(LightComponent* light : lights) {
                if(!light->enabled) continue;

                std::string prefix = "lights[" + std::to_string(light_index) + "].";
                 opaqueCommand.material->shader->set(prefix + "type", static_cast<int>(light->typeLight));
                auto lightPosition = glm::vec3((light)->getOwner()->getLocalToWorldMatrix() *
                                               glm::vec4((light)->getOwner()->localTransform.position, 1.0));
                switch(light->typeLight) {
                    case LightType::DIRECTIONAL:
                        opaqueCommand.material->shader->set(prefix + "direction", light->direction);
                        opaqueCommand.material->shader->set(prefix + "color" , light->color);
                        break;
                    case LightType::POINT:
                         opaqueCommand.material->shader->set(prefix + "position", lightPosition);

                        opaqueCommand.material->shader->set(prefix + "attenuation", glm::vec3(light->attenuation.quadratic,
                                                                                              light->attenuation.linear, light->attenuation.constant));
                        opaqueCommand.material->shader->set(prefix + "color" , light->color);
                        break;
                    case LightType::SPOT:
                        opaqueCommand.material->shader->set(prefix + "position", lightPosition);
                        opaqueCommand.material->shader->set(prefix + "direction", light->direction);

                        opaqueCommand.material->shader->set(prefix + "attenuation", glm::vec3(light->attenuation.quadratic,
                                                                                              light->attenuation.linear, light->attenuation.constant));
                        opaqueCommand.material->shader->set(prefix + "cone_angles", glm::vec2(light->spot_angle.inner, light->spot_angle.outer));
                        opaqueCommand.material->shader->set(prefix + "color" , light->color);
                        break;
                    case LightType::SKY:
                        break;
                    default:
                       h++;
                }
                light_index++;
                if (light_index >= MAX_LIGHT_COUNT)
                    break;
            }
          

            opaqueCommand.mesh->draw();
        }

      

        // If there is a sky material, draw the sky
            if (this->skyMaterial) {
                //TODO: (Req 10) setup the sky material
                skyMaterial->setup();

                //TODO: (Req 10) Get the camera position
                // ==> The camera position is the origin of the world in camera space
                glm::vec3 cameraPosition = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);

                //TODO: (Req 10) Create a model matrix for the sky such that it always follows the camera (sky sphere center = camera position)
                // ==> The sky sphere is centered at the camera position
                glm::mat4 skyModelMatrix = glm::translate(glm::mat4(1.0f), cameraPosition);

                //TODO: (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
                // We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
                // ==> The sky sphere is always behind everything (in NDC space, z=1)
                // ==> to make the sky sphere always behind everything
                // make z = 0 then translate it to z = 1 to make it the farthest object in the scene

                // x=x, y=y, z=1
                // 1 0 0 0     x     x
                // 0 1 0 0  *  y  =  y
                // 0 0 0 1     z     1
                // 0 0 0 1     1     1

                glm::mat4 alwaysBehindTransform = glm::mat4(
                        //  Row1, Row2, Row3, Row4
                        1.0, 0.0f, 0.0f, 0.0f,  // Column1
                        0.0f, 1.0f, 0.0f, 0.0f, // Column2
                        0.0f, 0.0f, 0.0f, 0.0f, // Column3
                        0.0f, 0.0f, 1.0f, 1.0f  // Column4
                );
                //TODO: (Req 10) set the "transform" uniform
                // ==> the matrix that we want to set is the product of the alwaysBehindTransform, the projection matrix, the view matrix, and the skyModelMatrix
                skyMaterial->shader->set("transform", alwaysBehindTransform * camera->getProjectionMatrix(windowSize) *
                                                      camera->getViewMatrix() * skyModelMatrix);

                //TODO: (Req 10) draw the sky sphere
                skySphere->draw();

            }
            //TODO: (Req 9) Draw all the transparent commands
            // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
            for (auto command: transparentCommands) {
                command.material->setup();
                command.material->shader->set("transform", VP * command.localToWorld);
                command.mesh->draw();
            }


            // If there is a postprocess material, apply postprocessing
            if (postprocessMaterial) {
                //TODO: (Req 11) Return to the default framebuffer
                // bind the default framebuffer
           ShaderProgram *postprocessShader = new ShaderProgram();
            // attach the vertex shader
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);

            // attach the fragment shader based on the effect type (fish eye or vignette)
            if ( effect )
                postprocessShader->attach("assets/shaders/postprocess/power_up.frag", GL_FRAGMENT_SHADER);
            else if(effect2)
                postprocessShader->attach("assets/shaders/postprocess/blur.frag", GL_FRAGMENT_SHADER);
            else if(effect3)
                 postprocessShader->attach("assets/shaders/postprocess/radial-blur.frag", GL_FRAGMENT_SHADER);
            else
                postprocessShader->attach("assets/shaders/postprocess/vignette.frag", GL_FRAGMENT_SHADER);

            // link the shader program
            postprocessShader->link();

            // create a postprocess material for the postprocess shader
            postprocessMaterial->shader = postprocessShader;

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // TODO: (Req 11) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            // draw the fullscreen triangle
            // ==> The fullscreen triangle is drawn using the postprocess shader
            // parameters:
            // 1. The type of primitive to render (GL_TRIANGLES)
            // 2. The starting index in the array
            // 3. The number of vertices to render
            glDrawArrays(GL_TRIANGLES, 0, 3);
            }
        }
    }

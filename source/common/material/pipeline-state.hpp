#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {
            //TODO: (Req 4) Write this function

            // ==> if daceCulling is enabled, enable face culling and set the culled face and front face
            // face culling:
            //  ==> if front face, the order is CCW, then show the front face
            //  ==> if back face, the order is CW, then don't show the back face
            //  ==> rotation 180 degree of front face around x or y axis will not affect back face
            //  ==> rotation 180 degree of back face around z axis will make back face become front face
            if (this->faceCulling.enabled) {
                // Enable face culling
                glEnable(GL_CULL_FACE);
                // Set the culled face
                glCullFace(this->faceCulling.culledFace);
                // Set the front face
                glFrontFace(this->faceCulling.frontFace);

            } else {
                // Disable face culling
                glDisable(GL_CULL_FACE);
            }

            // ==> if depthTesting is enabled, enable depth testing and set the depth function
            // depth testing:
            //  ==> if depth function is GL_LEQUAL, then the fragment will be drawn if the depth value of the fragment is less than or equal to the depth value stored in the depth buffer
            if (this->depthTesting.enabled) {
                // Enable depth testing
                glEnable(GL_DEPTH_TEST);
                // Set the depth function
                glDepthFunc(this->depthTesting.function);
            } else {
                // Disable depth testing
                glDisable(GL_DEPTH_TEST);
            }

            // ==> if blending is enabled, enable blending and set the blending equation, blending factors and blending constant color
            // blending:
            //  ==> if blending equation is GL_FUNC_ADD, then the source and destination data are added to each other
            if (this->blending.enabled) {
                // Enable blending
                glEnable(GL_BLEND);
                // Set the blending equation
                glBlendEquation(this->blending.equation);
                // Set the blending factors 
                glBlendFunc(this->blending.sourceFactor, this->blending.destinationFactor);
                // Set the blending constant color
                glBlendColor(this->blending.constantColor.r, this->blending.constantColor.g, this->blending.constantColor.b, this->blending.constantColor.a);
            } else {
                // Disable blending
                glDisable(GL_BLEND);
            }

            // Set the color mask
            glColorMask(colorMask.r, colorMask.g, colorMask.b, colorMask.a);
            // Set the depth mask
            glDepthMask(depthMask);
        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}
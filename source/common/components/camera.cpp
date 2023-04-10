#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace our
{
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if (cameraTypeStr == "orthographic")
        {
            cameraType = CameraType::ORTHOGRAPHIC;
        }
        else
        {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const
    {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        // TODO: (Req 8) Complete this function
        // HINT:
        //  In the camera space:
        //  - eye is the origin (0,0,0)
        //  - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        //  - up is the direction (0,1,0)
        //  but to use glm::lookAt, we need eye, center and up in the world state.
        //  Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        //  - the eye position which is the point (0,0,0) but after being transformed by M
        //  - the center position which is the point (0,0,-1) but after being transformed by M
        //  - the up direction which is the vector (0,1,0) but after being transformed by M
        //  then you can use glm::lookAt
        glm::vec3 eye = glm::vec3(M * glm::vec4(0, 0, 0, 1));
        glm::vec3 center = glm::vec3(M * glm::vec4(0, 0, -1, 1)); // as its point w=1
        glm::vec3 up = glm::vec3(M * glm::vec4(0, 1, 0, 0));      // as its vector w =0
        glm::mat4 view = glm::lookAt(eye, center, up);

        return view;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const
    {
        // TODO: (Req 8) Wrtie this function
        //  NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        //  It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        //  Left and Right are the same but after being multiplied by the aspect ratio
        //  For the perspective camera, you can use glm::perspective
        float aspectRatio = (float)viewportSize.x / viewportSize.y;
        if (cameraType == CameraType::ORTHOGRAPHIC)
        {
            return glm::ortho((-orthoHeight / 2) * aspectRatio, (orthoHeight / 2) * aspectRatio, -orthoHeight / 2, orthoHeight / 2);
        }
        else
        {
            //  For the perspective camera, you can use glm::perspective
            // fovY: The field of view angle in the vertical direction, specified in degrees. This is the angle between the top and bottom of the viewing frustum. A larger value will result in a wider field of view, and a smaller value will result in a narrower field of view.
            // aspectRatio: The ratio of the width to the height of the viewing frustum. This value should be set to the aspect ratio of the viewport or window where the scene will be rendered.
            // near: The distance to the near clipping plane of the viewing frustum. Any objects closer to the camera than this distance will be clipped and not rendered.
            // far: The distance to the far clipping plane of the viewing frustum. Any objects farther away from the camera than this distance will be clipped and not rendered.
            return glm::perspective(fovY, aspectRatio, near, far);
        }
        }
}
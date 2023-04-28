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

         /******************************************************************************************************
            This line creates a 3D vector representing the position of the camera, or "eye". 
            It does this by multiplying the model matrix M by a 4D vector (0, 0, 0, 1) that represents the origin of the scene in homogeneous coordinates. 
            The resulting vector is then truncated to a 3D vector.
        ******************************************************************************************************/
        glm::vec3 eye = glm::vec3(M * glm::vec4(0, 0, 0, 1));
        
        /******************************************************************************************************
            This line creates a 3D vector representing the point in the scene that the camera is looking at, or "center". 
            It does this by multiplying the model matrix M by a 4D vector (0, 0, -1, 1) that represents a point one unit in front of the camera along the negative z-axis.
            The resulting vector is then truncated to a 3D vector.
        ******************************************************************************************************/
        glm::vec3 center = glm::vec3(M * glm::vec4(0, 0, -1, 1)); // as its point w=1
        
        /******************************************************************************************************
            This line creates a 3D vector representing the direction that is "up" relative to the camera, or "up". 
            It does this by multiplying the model matrix M by a 4D vector (0, 1, 0, 0) that represents the y-axis direction in homogeneous coordinates. 
            Since the resulting vector is a direction and not a point,
            it is not affected by the translation component of the model matrix.
        ******************************************************************************************************/
        glm::vec3 up = glm::vec3(M * glm::vec4(0, 1, 0, 0));      // as its vector w =0
        
        /******************************************************************************************************
           generates a view matrix that simulates a camera positioned at eye looking towards the point center, 
           with the vector up indicating the camera's "up" direction. The resulting view matrix transforms objects from world space to camera space,
           making them appear as if they are viewed through the camera. 

           Parameters:
                eye: a glm::vec3 representing the camera's position in world space.
                center: a glm::vec3 representing the point in world space that the camera is looking at.
                up: a glm::vec3 representing the "up" direction of the camera.
            Return value:
            The function returns a glm::mat4 representing the resulting view matrix.
        ******************************************************************************************************/
        glm::mat4 view = glm::lookAt(eye, center, up);

        return view;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio

    /******************************************************************************************************
    This Function calculates and returns the projection matrix for the camera based on its cameraType, orthoHeight, fovY, near, far and the viewportSize of the camera. 
    The projection matrix transforms vertices from camera space to clip space,
    which is then used to perform perspective or orthographic projection.

    Parameters:
        viewportSize: a glm::ivec2 representing the viewport size of the camera.
    Return value:
        The function returns a glm::mat4 representing the calculated projection matrix.
    ******************************************************************************************************/
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
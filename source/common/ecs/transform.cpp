#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>
#include "glm/ext/matrix_transform.hpp"
#include <glm/gtx/quaternion.hpp>


namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        // 1. Compute the translation matrix
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        // 2. Compute the rotation matrix
        glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        // 3. Compute the scaling matrix
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scale);
        // 4. Compute the final matrix
        return translationMatrix * rotationMatrix * scalingMatrix;    
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}
#include "mesh-renderer.hpp"
#include "../asset-loader.hpp"
#include <string>
namespace our
{
    // Receives the mesh & material from the AssetLoader by the names given in the json object
    void MeshRendererComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        // Notice how we just get a string from the json file and pass it to the AssetLoader to get us the actual asset
        // TODO: (Req 8) Get the material and the mesh from the AssetLoader by their names
        // which are defined with the keys "mesh" and "material" in data.
        // Hint: To get a value of type T from a json object "data" where the key corresponding to the value is "key",
        // you can use write: data["key"].get<T>().
        // Look at "source/common/asset-loader.hpp" to know how to use the static class AssetLoader.
        /******************************************************************************************************
         Uses the AssetLoader class to load the mesh and material of the component using their names, which are defined in the data object.
         Specifically, it retrieves the value associated with the key "mesh" using data["mesh"].get<std::string>(),
         which gets the name of the mesh as a std::string. Similarly, it retrieves the value associated with the key "material" using data["material"].get<std::string>(), 
         which gets the name of the material as a std::string.
        ******************************************************************************************************/
        mesh = AssetLoader<Mesh>::get(data["mesh"].get<std::string>());
        material = AssetLoader<Material>::get(data["material"].get<std::string>());
    }
}
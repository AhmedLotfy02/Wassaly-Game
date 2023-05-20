#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {
        // TODO: (Req 7) Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {
        // TODO: (Req 7) Write this function
        Material::setup();
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {
        // TODO: (Req 7) Write this function
        TintedMaterial::setup();
        shader->set("alphaThreshold", alphaThreshold);
        // the texture unit number is 0
        shader->set("tex", 0);
        // by default texture is binded to texuture 0
        if(texture)
            texture->bind();
        else
            Texture2D::unbind();
        // bind the smapler to texture unit 0
        if( sampler)
            sampler->bind(0);
        else
            Sampler::unbind(0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

        // This function read the material data from a json object
    void LightMaterial::deserialize(const nlohmann::json& data) {
       
        TexturedMaterial::deserialize(data);
        if(!data.is_object()) return;
        albedo_map = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular_map = AssetLoader<Texture2D>::get(data.value("specular", ""));
        emissive_map = AssetLoader<Texture2D>::get(data.value("emissive", ""));
        roughness_map = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        ambient_occlusion_map =  AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void  LightMaterial::setup() const {

        TexturedMaterial::setup();
        if(albedo_map != nullptr)
        {
            glActiveTexture(GL_TEXTURE0); // activate texture unit 0
            albedo_map->bind();               // bind texture to texture2D
            shader->set("material.albedo", 0);
            sampler->bind(0);
        }

        if(specular_map != nullptr)
        {
            glActiveTexture(GL_TEXTURE1); // activate texture unit 1
            specular_map->bind();               // bind texture to texture2D
            shader->set("material.specular", 1);
            sampler->bind(1);
        }

        if(emissive_map != nullptr)
        {
            glActiveTexture(GL_TEXTURE2); // activate texture unit 2
            emissive_map->bind();               // bind texture to texture2D
            shader->set("material.emissive", 2);
            sampler->bind(2);
        }

        if(ambient_occlusion_map != nullptr)
        {
            // select an active texture unit -> 3
            glActiveTexture(GL_TEXTURE3); // activate texture unit 3
            ambient_occlusion_map->bind();               // bind texture to texture2D
            shader->set("material.ambient_occlusion", 3);
            sampler->bind(3);
        }

        if(roughness_map != nullptr)
        {
            // select an active texture unit -> 4
            glActiveTexture(GL_TEXTURE4); // activate texture unit 4
            roughness_map->bind();               // bind texture to texture2D
            shader->set("material.roughness", 4);
            sampler->bind(4);
        }

        glActiveTexture(GL_TEXTURE0);
    }

}
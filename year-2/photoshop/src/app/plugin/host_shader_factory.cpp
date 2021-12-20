#include <SFML/Graphics/Shader.hpp>
#include "host_shader_factory.hpp"

PUPPY::Shader* HostAppShaderFactory::compile(const char* code, PUPPY::ShaderType type, bool is_code) const {
    sf::Shader *shader = new sf::Shader;
    sf::Shader::Type rtype = sf::Shader::Type::Fragment;

    switch (type) {
        case PUPPY::ShaderType::FRAGMENT: rtype = sf::Shader::Type::Fragment; break;
        case PUPPY::ShaderType::VERTEX: rtype = sf::Shader::Type::Vertex; break;
        default: return nullptr;
    }

    if (!shader->loadFromMemory(code, rtype)) {
        delete shader;
        return nullptr;
    }

    shader->setUniform("texture", sf::Shader::CurrentTexture);
    return new HostAppShader(shader);
}

void HostAppShader::set_uniform_int(const char* name, int val) {
    shader->setUniform(name, val);
}

void HostAppShader::set_uniform_int_arr(const char* name, int* val, size_t cnt) {
    // Unimplemented, sorry
}

void HostAppShader::set_uniform_float(const char* name, float val) {
    shader->setUniform(name, val);
}

void HostAppShader::set_uniform_float_arr(const char* name, float* val, size_t cnt) {
    shader->setUniformArray(name, val, cnt);
}

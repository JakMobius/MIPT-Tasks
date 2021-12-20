#pragma once

#include "plugin_std.hpp"
#include "app_interface.hpp"

class HostAppShader : public PUPPY::Shader {
    sf::Shader* shader;
public:
    HostAppShader(sf::Shader* shader): shader(shader) {};
    ~HostAppShader() { delete shader; }

    void set_uniform_int(const char* name, int val) override;
    void set_uniform_int_arr(const char* name, int* val, size_t cnt) override;

    void set_uniform_float(const char* name, float val) override;
    void set_uniform_float_arr(const char* name, float* val, size_t cnt) override;

    sf::Shader* get_shader() const { return shader; }
};

class HostAppShaderFactory : public PUPPY::ShaderFactory {
    HostAppInterface* interface = nullptr;
public:
    HostAppShaderFactory(HostAppInterface* interface): interface(interface) {};
    PUPPY::Shader* compile(const char* code, PUPPY::ShaderType type, bool is_code) const override;

};
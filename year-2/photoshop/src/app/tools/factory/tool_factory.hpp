#pragma once

class ToolFactoryBase;

#include "../../../graphics/drawable_texture.hpp"
#include "../tool.hpp"

class ToolFactoryBase {
protected:
    const char* tool_name = nullptr;
    DrawableTexture* tool_icon = nullptr;
public:
    virtual Tool* create_tool() = 0;

    const char* get_tool_name() const { return tool_name; }
    DrawableTexture* get_tool_icon() const { return tool_icon; }
};

template<typename ToolType>
class ToolFactory : public ToolFactoryBase {
public:
    ToolFactory(const char* name, DrawableTexture* icon) {
        tool_name = name;
        tool_icon = icon;
    };
    Tool* create_tool() { return new ToolType(); }
};
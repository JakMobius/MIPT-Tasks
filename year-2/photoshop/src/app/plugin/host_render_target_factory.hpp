#pragma once

#include "plugin_std.hpp"
#include "app_interface.hpp"

class HostRenderTargetFactory : public PUPPY::RenderTargetFactory {
    HostAppInterface* interface;
public:

    explicit HostRenderTargetFactory(HostAppInterface* interface): interface(interface) {}

    PUPPY::RenderTarget* spawn(const PUPPY::Vec2s &size, const PUPPY::RGBA &color) const override;

    PUPPY::RenderTarget* from_pixels(const PUPPY::Vec2s &size, const PUPPY::RGBA* data) const override;

    PUPPY::RenderTarget* from_file(const char* path) const override;

};
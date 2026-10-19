#pragma once

#include "mods/svc/texture.h"
#include "mods/svc/config.h"
#include <list>
#include <optional>
#include <string>
#include <vector>
#include <gx.h>

struct TextureReplacementData {
    const char* arc{};
    const char* modelFileName{};
    const char* textureName{};
    uint64_t textureHash{};
    uint64_t tlutHash{};
    TextureKey key = TEXTURE_KEY_INIT;
    TextureData data = TEXTURE_DATA_INIT;
    TextureReplacementHandle handle{};
    std::vector<u8> baseTextureData{};
    bool loadedTextureData = false;
    std::optional<GXColor> curColor{};
};

uint32_t get_image_data_size(
    uint32_t format, uint32_t width, uint32_t height, uint32_t mipmapCount);

void recolor_texture(
    TextureReplacementData& replacementData, GXColor color, std::vector<u8>& newTextureDataOut);

std::list<TextureReplacementData>& get_texture_replacements();

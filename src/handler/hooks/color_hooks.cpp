#include "color_hooks.hpp"
#include "hooks.hpp"
#include "d/actor/d_a_midna.h"
#include "texture_utils.hpp"
#include <cstdlib>
#include <xxhash.h>
#include <ranges>

ColorHooks* g_color;

IMPORT_SERVICE(TextureService, svc_texture);

// Random Color Helpers
GXColor randomColor() {
    return {
        static_cast<u8>(std::rand() % 256),
        static_cast<u8>(std::rand() % 256),
        static_cast<u8>(std::rand() % 256),
        0
    };
}

GXColorS10 randomColorS10() {
    return {
        static_cast<s16>(std::rand() % 256),
        static_cast<s16>(std::rand() % 256),
        static_cast<s16>(std::rand() % 256),
        0
    };
}

// Midna Hair Colors
namespace MidnaHair {

DEFINE_HOOK(&daMidna_c::create, MidnaCreate);
void randomizeMidnaHairColors() {
        g_color->getMidnaHairColors()->normalColor = randomColorS10();
        g_color->getMidnaHairColors()->normalKColor = randomColor();
        g_color->getMidnaHairColors()->normalKColor2 = randomColor();
        g_color->getMidnaHairColors()->bigColor = randomColorS10();
        g_color->getMidnaHairColors()->bigKColor = randomColor();
        g_color->getMidnaHairColors()->lNormalKColor = randomColor();
        g_color->getMidnaHairColors()->lNormalKColor2 = randomColor();
        g_color->getMidnaHairColors()->lBigColor = randomColorS10();
        g_color->getMidnaHairColors()->lBigKColor2 = randomColor();
}
void onMidnaCreate(ModContext*, void* args, void* retval, void*) {
    if (GET_CVAR_VALUE(g_color, RandoColors) == true) {
        randomizeMidnaHairColors();
    } else {
        g_color->getMidnaHairColors()->setDefaults();
    }

    auto step = reinterpret_cast<int*>(retval);
    // Don't set colors if midna isn't done loading
    if (*step != cPhs_COMPLEATE_e) {
        return;
    }

    auto midna = mods::arg<daMidna_c*>(args, 0);
    midna->field_0x6e0 = g_color->getMidnaHairColors()->normalColor;
    if (dKy_darkworld_check()) {
        midna->field_0x6e8 = g_color->getMidnaHairColors()->normalKColor;
        midna->field_0x6ec = g_color->getMidnaHairColors()->normalKColor2;
    } else {
        midna->field_0x6e8 = g_color->getMidnaHairColors()->lNormalKColor;
        midna->field_0x6ec = g_color->getMidnaHairColors()->lNormalKColor2;
    }
}

DEFINE_HOOK(&daMidna_c::setBodyPartMatrix, MidnaSetBodyPartMatrix);
static GXColorS10 midnaField0x6e0{};
static GXColor midnaField0x6e8{};
static GXColor midnaField0x6ec{};
// Copy the original values before setBodyPartMatrix runs
HookAction onMidnaSetBodyPartMatrixPre(ModContext*, void* args, void*, void*) {
    auto midna = mods::arg<daMidna_c*>(args, 0);

    midnaField0x6e0 = midna->field_0x6e0;
    midnaField0x6e8 = midna->field_0x6e8;
    midnaField0x6ec = midna->field_0x6ec;

    return HOOK_CONTINUE;
}

void onMidnaSetBodyPartMatrixPost(ModContext*, void* args, void* retval, void* userdata) {
    auto midna = mods::arg<daMidna_c*>(args, 0);
    if (midna->mpHairhandBmd != NULL) {
        // Restore the original values from before setBodyPartMatrix ran (this undoes the chase)
        midna->field_0x6e0 = midnaField0x6e0;
        midna->field_0x6e8 = midnaField0x6e8;
        midna->field_0x6ec = midnaField0x6ec;

        // Statement copied from inside function to determine colors
        bool bigColors =
            midna->checkStateFlg0(daMidna_c::FLG0_UNK_10000000) ||
            midna->mBckHeap[2].getIdx() == daMidna_c::m_anmDataTable[daMidna_c::ANM_HAIR].mResID ||
            midna->mBckHeap[2].getIdx() ==
                daMidna_c::m_anmDataTable[daMidna_c::ANM_S_TAKES].mResID ||
            midna->mBckHeap[2].getIdx() ==
                daMidna_c::m_anmDataTable[daMidna_c::ANM_S_WAITS].mResID ||
            midna->mBckHeap[2].getIdx() ==
                daMidna_c::m_anmDataTable[daMidna_c::ANM_S_PACKAWAY].mResID ||
            midna->mBckHeap[2].getIdx() ==
                daMidna_c::m_anmDataTable[daMidna_c::ANM_GRABST].mResID ||
            midna->checkEndResetStateFlg0(daMidna_c::ERFLG0_UNK_40) ||
            dComIfGp_checkPlayerStatus1(0, 0x800000);

        GXColorS10 color{};
        GXColor kcolor1{};
        GXColor kcolor2{};

        // Set our own colors
        if (bigColors) {
            kcolor1 = g_color->getMidnaHairColors()->bigKColor;
            if (dKy_darkworld_check()) {
                color = g_color->getMidnaHairColors()->bigColor;
                kcolor2 = g_color->getMidnaHairColors()->normalKColor2;
            } else {
                color = g_color->getMidnaHairColors()->lBigColor;
                kcolor2 = g_color->getMidnaHairColors()->lBigKColor2;
            }
        } else {
            color = g_color->getMidnaHairColors()->normalColor;
            if (dKy_darkworld_check()) {
                kcolor1 = g_color->getMidnaHairColors()->normalKColor;
                kcolor2 = g_color->getMidnaHairColors()->normalKColor2;
            } else {
                kcolor1 = g_color->getMidnaHairColors()->lNormalKColor;
                kcolor2 = g_color->getMidnaHairColors()->lNormalKColor2;
            }
        }

        // Reapply the chase that happens in the function
        cLib_chaseS(&midna->field_0x6e0.r, color.r, 10);
        cLib_chaseS(&midna->field_0x6e0.g, color.g, 10);
        cLib_chaseS(&midna->field_0x6e0.b, color.b, 10);
        cLib_chaseUC(&midna->field_0x6e8.r, kcolor1.r, 10);
        cLib_chaseUC(&midna->field_0x6e8.g, kcolor1.g, 10);
        cLib_chaseUC(&midna->field_0x6e8.b, kcolor1.b, 10);
        cLib_chaseUC(&midna->field_0x6ec.r, kcolor2.r, 10);
        cLib_chaseUC(&midna->field_0x6ec.g, kcolor2.g, 10);
        cLib_chaseUC(&midna->field_0x6ec.b, kcolor2.b, 10);
    }
}

};

bool g_loadedAllBaseTextures = false;
constexpr uint32_t kTextureLoadRetryFrames = 30;
uint32_t g_textureLoadRetryCountdown = 0;
void load_base_texture_data() {
    // Go through each texture we can recolor and attempt to load and store
    // the texture data for future recoloring
    for (auto& replacement : get_texture_replacements()) {
        // If we've already loaded the texture data, don't load it again
        if (replacement.loadedTextureData) {
            continue;
        }

        // Avoid noisy resource lookups until the archive has finished loading.
        auto* resInfo = dComIfG_getObjectResInfo(replacement.arc);
        if (resInfo == nullptr || resInfo->getArchive() == nullptr) {
            continue;
        }

        // Try to get the model this texture is part of. If we can't get it, try again later
        auto model = static_cast<J3DModelData*>(
            dComIfG_getObjectRes(replacement.arc, replacement.modelFileName));
        if (model == nullptr) {
            continue;
        }

        J3DTexture* tex = model->getTexture();
        JUTNameTab* nametable = model->getTextureName();
        if (tex != nullptr && nametable != nullptr) {
            for (u16 i = 0; i < tex->getNum(); i++) {
                const char* texName = nametable->getName(i);
                if (texName != nullptr && std::strcmp(texName, replacement.textureName) == 0) {
                    // Once we've found the texture, set all our TextureKey and TextureData
                    // fields that we can set right now.
                    auto imageHeader = tex->getResTIMG(i);
                    auto& key = replacement.key;
                    auto& data = replacement.data;
                    key.kind = TEXTURE_KEY_SOURCE;
                    key.has_tlut = imageHeader->numColors > 0;
                    key.width = imageHeader->width;
                    key.height = imageHeader->height;
                    key.gx_format = imageHeader->format;

                    // Currently, no replaced textures have a tlut
                    key.tlut_hash = replacement.tlutHash;

                    // Calculate the size of the image data
                    const uint32_t mipCount =
                        imageHeader->mipmapEnabled ?
                            std::max<uint32_t>(imageHeader->mipmapCount, 1) :
                            1;
                    auto size = get_image_data_size(
                        imageHeader->format, imageHeader->width, imageHeader->height, mipCount);
                    replacement.baseTextureData.resize(size);
                    std::memcpy(
                        replacement.baseTextureData.data(), tex->getImgDataPtr(i), size);

                    // Source texture keys hash only the base mip level.
                    const auto baseMipSize = get_image_data_size(
                        imageHeader->format, imageHeader->width, imageHeader->height, 1);
                    auto textureHash =
                        XXH64(replacement.baseTextureData.data(), baseMipSize, 0);
                    replacement.key.texture_hash = textureHash;

                    mods::log::debug("Loaded base texture data for {}. size: {:X} hash: {:X}",
                        replacement.textureName, size, textureHash);
                    replacement.loadedTextureData = true;

                    data.width = imageHeader->width;
                    data.height = imageHeader->height;
                    data.mip_count = mipCount;
                    data.size = size;
                    data.gx_format = imageHeader->format;

                    break;
                }
            }
        }
    }

    // If we've loaded all base textures for recoloring, then we don't need to call this function
    // again
    g_loadedAllBaseTextures = std::ranges::all_of(
        get_texture_replacements(),
        [](const TextureReplacementData& replacement) {
            return replacement.loadedTextureData;
        });
}


ModResult ColorHooks::initialize(ConfigHandler* config_handler, ModError* error) {
    configHandler = config_handler;
    g_color = this;
    g_loadedAllBaseTextures = false;
    g_textureLoadRetryCountdown = 0;
    ModResult result;

    POST_HOOK(MidnaHair::MidnaCreate, MidnaHair::onMidnaCreate, "Failed to hook daMidna_c::create");
    PRE_HOOK(MidnaHair::MidnaSetBodyPartMatrix, MidnaHair::onMidnaSetBodyPartMatrixPre, "Failed to hook daMidna_c::setBodyPartMatrix");
    POST_HOOK(MidnaHair::MidnaSetBodyPartMatrix, MidnaHair::onMidnaSetBodyPartMatrixPost, "Failed to hook daMidna_c::setBodyPartMatrix");

    return result;
}

void ColorHooks::update() {
    if (!g_loadedAllBaseTextures) {
        if (g_textureLoadRetryCountdown == 0) {
            load_base_texture_data();
            g_textureLoadRetryCountdown = kTextureLoadRetryFrames;
        } else {
            --g_textureLoadRetryCountdown;
        }
    }
}
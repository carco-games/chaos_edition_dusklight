#pragma once

#include "../../context.hpp"
#include "../config/config.hpp"
#include "mods/svc/hook.hpp"
#include <gx.h>

class ColorHooks {
public:
    struct MidnaHairColors {
        GXColorS10 normalColor;
        GXColor normalKColor;
        GXColor normalKColor2;
        GXColorS10 bigColor;
        GXColor bigKColor;
        GXColor lNormalKColor;
        GXColor lNormalKColor2;
        GXColorS10 lBigColor;
        GXColor lBigKColor2;

        void setDefaults() {
            normalColor = {0x50, 0x00, 0x00, 0x00};
            normalKColor = {0xB4, 0x87, 0x00, 0x00};
            normalKColor2 = {0x00, 0xC3, 0xC3, 0x00};
            bigColor = {0xFF, 0x64, 0x78, 0x00};
            bigKColor = {0x1E, 0x00, 0x00, 0x00};
            lNormalKColor = {0xFF, 0xDC, 0x00, 0x00};
            lNormalKColor2 = {0x00, 0xC3, 0xEB, 0x00};
            lBigColor = {0xFF, 0x78, 0x00, 0x00};
            lBigKColor2 = {0xAA, 0xFF, 0xC3, 0x00};
        }
    };
    ModResult initialize(ConfigHandler* config_handler, ModError* error);
    ConfigHandler* getConfigHandler() { return configHandler; }
    MidnaHairColors* getMidnaHairColors() { return &midnaHairColors; }
    void update();
private:
    ConfigHandler* configHandler;
    MidnaHairColors midnaHairColors;
};
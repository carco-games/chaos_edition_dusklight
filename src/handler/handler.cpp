#include "handler.hpp"

ModResult Handler::initialize(ModError* error) {
    ModResult result;

    INITIALIZE(configHandler)

    result = hooksHandler.initialize(&configHandler, error);
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error in hooksHandler");
    }

    result = uiHandler.initialize(&configHandler, error);
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error in uiHandler");
    }

    return result;
}
#include "../../context.hpp"
#include "../config/config.hpp"
#include <mods/svc/ui.hpp>

class UiHandler {
public:
    ModResult initialize(ConfigHandler* config_handler, ModError* error);
    ConfigHandler* getConfigHandler() { return configHandler; }
private:
    ConfigHandler* configHandler;
};
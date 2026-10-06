#include "../context.hpp"
#include "config/config.hpp"
#include "hooks/hooks.hpp"
#include "ui/ui.hpp"

class Handler {
public:
    ModResult initialize(ModError* error);
    HooksHandler* getHooksHandler() { return &hooksHandler; }
private:
    ConfigHandler configHandler;
    HooksHandler hooksHandler;
    UiHandler uiHandler;
};
#include "../context.hpp"
#include "config/config.hpp"
#include "hooks/hooks.hpp"
#include "ui/ui.hpp"

class Handler {
public:
    ModResult initialize(ModError* error);
private:
    ConfigHandler configHandler;
    HooksHandler hooksHandler;
    UiHandler uiHandler;
};
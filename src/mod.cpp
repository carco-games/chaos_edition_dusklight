#include "context.hpp"
#include "handler/handler.hpp"

DEFINE_MOD();

IMPORT_SERVICE(LogService, svc_log);

Handler g_handler;

extern "C" {
MOD_EXPORT ModResult mod_initialize(ModError* error) {
    ModResult result;
    INITIALIZE(g_handler);
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    return MOD_OK;
}
}

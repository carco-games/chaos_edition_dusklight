cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED ENV{CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Build type")
endif ()

project(chaos_edition VERSION 0.1.0 LANGUAGES CXX)

# The Dusklight version to build against: a release tag (e.g. "v2.0.0") or a commit SHA.
if (NOT DUSKLIGHT_VERSION)
    set(DUSKLIGHT_VERSION "v2.0.2")
endif ()
# Fetches the Dusklight sources into ./dusklight and provides DUSKLIGHT_DIR.
# Use -DDUSKLIGHT_DIR= to point at an existing Dusklight checkout.
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetchDusklight.cmake")
add_subdirectory("${DUSKLIGHT_DIR}/sdk" dusklight-sdk EXCLUDE_FROM_ALL)

configure_file(mod.json.in "${CMAKE_CURRENT_BINARY_DIR}/mod.json" @ONLY NEWLINE_STYLE LF)

add_mod(chaos_edition
        FEATURES game fmt          # remove for service/asset-only mods; add webgpu for GfxService
        SOURCES
            src/mod.cpp
            src/handler/handler.cpp
            src/handler/config/config.cpp
            src/handler/hooks/hooks.cpp
            src/handler/ui/ui.cpp
        MOD_JSON "${CMAKE_CURRENT_BINARY_DIR}/mod.json"
)

# For Faster Testing
add_custom_target(copy_mod ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$ENV{APPDATA}/TwilitRealm/Dusklight/mods"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/mods/chaos_edition.dusk"
        "$ENV{APPDATA}/TwilitRealm/Dusklight/mods/chaos_edition.dusk"

    DEPENDS package_mod

    COMMENT "Copying chaos_edition.dusk to Dusklight mods folder"
)
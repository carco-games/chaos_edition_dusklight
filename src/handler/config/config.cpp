#include "config.hpp"

IMPORT_SERVICE(ConfigService, svc_config);

#define EFFECT_CVAR(name, type, default)    \
    ConfigVarHandle g_cvar##name;           \
    ConfigVarHandle g_cvar##name##Shuffle;

#define CVAR(name, type, default)   \
    ConfigVarHandle g_cvar##name;

CHAOS_EFFECTS(EFFECT_CVAR);
EFFECT_SETTINGS(CVAR);
SHUFFLE_SETTINGS(CVAR);

#define MAP_EFFECT_CVAR(name)               \
    name = &g_cvar##name;                   \
    name##Shuffle = &g_cvar##name##Shuffle;

#define MAP_CVAR(name)      \
    name = &g_cvar##name;

ModResult ConfigHandler::initialize(ModError* error) {
    ModResult result;
    CHAOS_EFFECTS(REGISTER_CVARS);
    CHAOS_EFFECTS(REGISTER_SHUFFLE_CVARS);
    EFFECT_SETTINGS(REGISTER_CVARS);
    SHUFFLE_SETTINGS(REGISTER_CVARS);

    CHAOS_EFFECTS(MAP_EFFECT_CVAR)
    EFFECT_SETTINGS(MAP_CVAR);
    SHUFFLE_SETTINGS(MAP_CVAR);
    return MOD_OK;
}

#define GET_SHUFFLE_EFFECT_ENABLED(name, type, default) \
    if (get##name##Shuffle() == true) {                 \
        validShuffleEffects.emplace_back(name);         \
    }

std::vector<ConfigVarHandle*> validShuffleEffects;
void ConfigHandler::shuffleEffects() {
    validShuffleEffects.clear();
    CHAOS_EFFECTS(GET_SHUFFLE_EFFECT_ENABLED);

    for (auto* effect : validShuffleEffects) {
        setOption<bool>(*effect, false);
    }

    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int64_t> dist(1, validShuffleEffects.size());
    int64_t numOfEffectsToEnable = dist(rng);
    std::uniform_int_distribution<int64_t> indexDist(0, validShuffleEffects.size() - 1);
    std::vector<int64_t> enabledIndexes;
    for (int64_t i = 0; i < numOfEffectsToEnable; i++) {
        bool activateEffect = true;
        while (activateEffect) {
            int64_t randomIndex = indexDist(rng);
            if (std::find(enabledIndexes.begin(), enabledIndexes.end(), randomIndex) == enabledIndexes.end()) {
                setOption<bool>(*validShuffleEffects[randomIndex], true);
                enabledIndexes.emplace_back(randomIndex);
                activateEffect = false;
            }
        }
    }

    // If both Fast Link and Arthritis are enabled, randomly disable one of them
    if (getFastLink() && getArthritis()) {
        std::uniform_int_distribution<int64_t> disableDist(0, 1);
        int64_t disable = disableDist(rng);
        switch (disable) {
            case 0:
                setCvarFastLink(false);
                break;

            case 1:
                setCvarArthritis(false);
                break;
        }
    }

    // If both Tiny and Big Link are enabled, randomly disable one of them
    if (getTinyLink() && getBigLink()) {
        std::uniform_int_distribution<int64_t> disableDist(0, 1);
        int64_t disable = disableDist(rng);
        switch (disable) {
            case 0:
                setCvarTinyLink(false);
                break;

            case 1:
                setCvarBigLink(false);
                break;
        }
    }

    // If both Zoom Out and Top-Down are enabled, randomly disable one of them
    if (getZoomOut() && getTopDown()) {
        std::uniform_int_distribution<int64_t> disableDist(0, 1);
        int64_t disable = disableDist(rng);
        switch (disable) {
            case 0:
                setCvarZoomOut(false);
                break;

            case 1:
                setCvarTopDown(false);
                break;
        }
    }

    shuffle = false;
}
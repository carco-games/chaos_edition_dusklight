#include "ui.hpp"

IMPORT_SERVICE(UiService, svc_ui);

UiHandler* g_ui;
UiWindowHandle g_settingsWindow;
UiWindowHandle g_effEnablesWindow;
UiWindowHandle g_effSettingsWindow;

ModResult add_control(UiElementHandle pane, const UiControlDesc& desc) {
    return svc_ui->pane_add_control(mod_ctx, pane, &desc, nullptr);
}

ModResult add_toggle(UiElementHandle pane, const char* label, ConfigVarHandle cvar, const char* help, UiPressedFn on_press = nullptr) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_TOGGLE;
    control.label = label;
    control.help_rml = help;
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = cvar;
    control.on_pressed = on_press;
    return add_control(pane, control);
}

void on_settings_close(ModContext*, UiWindowHandle, void*) {
    g_settingsWindow = 0;
}

void on_eff_settings_close(ModContext*, UiWindowHandle, void*) {
    g_effSettingsWindow = 0;
}

void on_eff_enables_close(ModContext*, UiWindowHandle, void*) {
    g_effEnablesWindow = 0;
}

void onEnableShuffleMode(ModContext*, void*) {
    if (GET_CVAR_VALUE(g_ui, ShuffleEnabled) == true) {
        SET_CVAR_VALUE(g_ui, BonkShuffleEnabled, false);
    }
}

ModResult buildShuffleModeTab(ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle, void*, ModError* error) {
    ModResult result = add_toggle(left, "Enable Shuffle Mode", GET_CVAR(g_ui, ShuffleEnabled), "Enable regular Shuffle Mode", onEnableShuffleMode);
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error adding Shuffle Mode Enabled toggle");
    }

    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_NUMBER;
    control.label = "Shuffle Timer";
    control.help_rml = "Timer denoting time between effect shuffles";
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = GET_CVAR(g_ui, ShuffleTimer);
    control.min = 1;
    result = add_control(left, control);
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error adding Shuffle Timer control");
    }

    result = add_toggle(left, "Enable Random Timer", GET_CVAR(g_ui, EnableRandTimer), "Enable randomized timer for Shuffle Mode");
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error adding Random Timer toggle");
    }

    control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_NUMBER;
    control.label = "Random Timer Upper Limit";
    control.help_rml = "Upper limit (in seconds) for random shuffle timer";
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = GET_CVAR(g_ui, RandTimerUpperLimit);
    control.min = 1;
    result = add_control(left, control);
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error adding Random Timer Upper Limit control");
    }

    result = add_toggle(left, "Shuffle Mode Toasts", GET_CVAR(g_ui, ShuffleToasts), "Enable toasts for regular Shuffle Mode");
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error adding Shuffle Mode Toasts toggle");
    }

    return result;
}

void onEnableBonkShuffleMode(ModContext*, void*) {
    mods::log::debug("bonk shuffle enabled");
    if (GET_CVAR_VALUE(g_ui, BonkShuffleEnabled) == true) {
        SET_CVAR_VALUE(g_ui, ShuffleEnabled, false);
    }
}

ModResult buildBonkShuffleModeTab(ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle, void*, ModError* error) {
    ModResult result = add_toggle(left, "Enable Bonk Shuffle Mode", GET_CVAR(g_ui, BonkShuffleEnabled), "Enable Bonk Shuffle Mode", onEnableBonkShuffleMode);
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error adding Bonk Shuffle Mode Enabled toggle");
    }

    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_NUMBER;
    control.label = "Bonk Shuffle Cooldown";
    control.help_rml = "Cooldown Timer (In Seconds) for Bonk Shuffling";
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = GET_CVAR(g_ui, BonkShuffleCooldown);
    control.min = 1;
    control.max = 900;
    result = add_control(left, control);
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error adding Bonk Shuffle Cooldown control");
    }

    result = add_toggle(left, "Enable Bonk Shuffle Toasts", GET_CVAR(g_ui, BonkShuffleToasts), "Enable toasts for Bonk Shuffle Mode");
    if (result != MOD_OK) {
        return mods::set_error(error, result, "Error adding Enable Bonk Shuffle Toasts toggle");
    }

    return result;
}

void onOpenShuffleSettings(ModContext*, void*) {
    if (g_settingsWindow != 0) {
        return;
    }

    UiTabDesc tabs[2] = {UI_TAB_DESC_INIT, UI_TAB_DESC_INIT};
    tabs[0].title = "Shuffle Mode";
    tabs[0].build = &buildShuffleModeTab;

    tabs[1].title = "Bonk Shuffle Mode";
    tabs[1].build = &buildBonkShuffleModeTab;

    ADD_TABS(2, on_settings_close, g_settingsWindow, "Failed to open settings");
}

ModResult buildEffEnabledTab(ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle, void*, ModError* error) {
    ModResult result;
    EFFECT_TOGGLES(ADD_TOGGLE);
    return result;
}

ModResult buildShuffleEffEnabledTab(ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle, void*, ModError* error) {
    ModResult result;
    EFFECT_TOGGLES(ADD_SHUFFLE_TOGGLE);
    return result;
}

void onOpenEffEnables(ModContext*, void*) {
    if (g_effEnablesWindow != 0) {
        return;
    }

    UiTabDesc tabs[2] = {UI_TAB_DESC_INIT, UI_TAB_DESC_INIT};
    tabs[0].title = "Effects Enabled";
    tabs[0].build = &buildEffEnabledTab;

    tabs[1].title = "Shuffle Mode Effects Enabled";
    tabs[1].build = &buildShuffleEffEnabledTab;

    ADD_TABS(2, on_eff_enables_close, g_effEnablesWindow, "Failed to effect enables");
}

ModResult buildEffSettingsTab(ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle, void*, ModError* error) {
    ModResult result;
    UiControlDesc control;
    ADD_NUMBER_CONTROL("Fast Link Speed", "Speed scale for 'Fast Link' effect", GET_CVAR(g_ui, FastLinkSpeed), 1, 5,
                       "Error adding Fast Link Speed control");

    ADD_NUMBER_CONTROL("Damage Roulette Odds", "Odds for 'Damage Roulette' effect to heal", GET_CVAR(g_ui, DamageRouletteOdds), 1, 100,
                       "Error adding Damage Roulette Odds control");

    ADD_NUMBER_CONTROL("Tiny Link Scale", "Scale for 'Tiny Link' effect", GET_CVAR(g_ui, TinyLinkScale), 1, 100,
                       "Error adding Tiny Link Scale control");

    ADD_NUMBER_CONTROL("Big Link Scale", "Scale for 'Big Link' effect", GET_CVAR(g_ui, BigLinkScale), 1, 100,
                       "Error adding Big Link Scale control");

    ADD_NUMBER_CONTROL("Slippery Scale", "Scale for 'Slippery' effect", GET_CVAR(g_ui, SlipperyScale), 1, 5,
                       "Error adding Slippery Scale control");

    ADD_NUMBER_CONTROL("Zoom Out Percent", "Zoom percentage for 'Zoom Out' effect", GET_CVAR(g_ui, ZoomOutPercent), 100, 200,
                       "Error adding Zoom Out Percent control");

    ADD_NUMBER_CONTROL("Health Drain Timer", "Timer (in seconds) for 'Health Drain' effect", GET_CVAR(g_ui, HealthDrainTimer), 1, 1800,
                       "Error adding Health Drain Timer control");

    ADD_NUMBER_CONTROL("Health Drain Amount", "Amount of life that 'Health Drain' effect should drain", GET_CVAR(g_ui, HealthDrainAmount), 1, 80,
                       "Error adding Health Drain Amount control");

    ADD_NUMBER_CONTROL("Spin Speed Scale", "Speed scale for 'Spin' effect", GET_CVAR(g_ui, SpinSpeed), 1, 5,
                       "Error adding Spin Speed Scale control");

    ADD_NUMBER_CONTROL("Drops Damage Amount", "Damage Amount for 'Drops Damage' effect", GET_CVAR(g_ui, DropsDamageAmount), 1, 80,
                       "Error adding Drops Damage Amount control");

    ADD_NUMBER_CONTROL("Exploding Enemies Timer", "Timer (in seconds) for 'Exploding Enemies' effect", GET_CVAR(g_ui, ExplodeEnemiesTimer), 1, 120,
                       "Error adding Exploding Enemies Timer control");

    return result;
}

void onOpenEffSettings(ModContext*, void*) {
    if (g_effSettingsWindow != 0) {
        return;
    }

    UiTabDesc tabs[1] = {UI_TAB_DESC_INIT};
    tabs[0].title = "Effects Section 1";
    tabs[0].build = &buildEffSettingsTab;

    ADD_TABS(1, on_eff_settings_close, g_effSettingsWindow, "Failed to open effects settings");
}

ModResult buildInitialPanel(ModContext*, UiElementHandle panel, void*, ModError* error) {
    UiControlDesc control;
    ModResult result;
    ADD_INIT_CONTROL(UI_CONTROL_BUTTON, "Settings", onOpenShuffleSettings, "Failed to add shuffle settings button");
    ADD_INIT_CONTROL(UI_CONTROL_BUTTON, "Effect Enables", onOpenEffEnables, "Failed to add effect enables button");
    ADD_INIT_CONTROL(UI_CONTROL_BUTTON, "Effect Settings", onOpenEffSettings, "Failed to add effect settings button");
}

ModResult UiHandler::initialize(ConfigHandler* config_handler, ModError* error) {
    configHandler = config_handler;
    g_ui = this;
    UiModsPanelDesc panelDesc = UI_MODS_PANEL_DESC_INIT;
    panelDesc.build = &buildInitialPanel;
    return svc_ui->register_mods_panel(mod_ctx, &panelDesc);
}
#pragma once

enum PropertyType {
    DEFENSE_TOWER,
    BARRACKS,
    ARCHERY,
    WIZARDRY,
    HOUSE,
    LIGHTNING_TOWER,
    CASTLE,
};

float getTributeRate(PropertyType type);
void initPropertyTributeRates();

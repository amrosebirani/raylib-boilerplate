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
float getHouseTributeRate(int level);
void initPropertyTributeRates();

#include "property_type.hpp"
#include <unordered_map>

std::unordered_map<PropertyType, float> property_tribute_rates;

void initPropertyTributeRates() {
    property_tribute_rates[PropertyType::DEFENSE_TOWER] = 0.15;
    property_tribute_rates[PropertyType::BARRACKS] = 0.2;
    property_tribute_rates[PropertyType::ARCHERY] = 0.2;
    property_tribute_rates[PropertyType::WIZARDRY] = 0.3;
    property_tribute_rates[PropertyType::HOUSE] = 0.4;
    property_tribute_rates[PropertyType::LIGHTNING_TOWER] = 0.15;
    property_tribute_rates[PropertyType::CASTLE] = 0.0;
}

float getTributeRate(PropertyType type) {
    return property_tribute_rates[type];
}

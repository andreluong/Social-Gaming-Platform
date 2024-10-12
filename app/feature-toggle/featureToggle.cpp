#include "include/featureToggle.h"

FeatureToggle::FeatureToggle() {
    loadDefaults();
}

void FeatureToggle::loadDefaults() {
    featureMap["feature1"] = true;  // default ON
    featureMap["feature2"] = false; // default OFF
}

void FeatureToggle::setEnabled(const std::string& feature, bool state) {
    featureMap[feature] = state;
}

bool FeatureToggle::isEnabled(const std::string& feature) const {
    auto it = featureMap.find(feature);
    return (it != featureMap.end()) ? it->second : false;
}
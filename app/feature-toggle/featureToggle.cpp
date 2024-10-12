#include "include/featureToggle.h"

FeatureToggle::FeatureToggle() {
    loadDefaults();
}

void FeatureToggle::loadDefaults() {
    featureMap["defaulton"] = true; // DEFAULT for testing
    featureMap["defaultoff"] = false; // DEFAULT for testing
    featureMap["feature1"] = true;
    featureMap["feature2"] = false;
}

void FeatureToggle::setEnabled(const std::string& feature, bool state) {
    featureMap[feature] = state;
}

bool FeatureToggle::isEnabled(const std::string& feature) const {
    auto it = featureMap.find(feature);
    return (it != featureMap.end()) ? it->second : false;
}

void FeatureToggle::reset() {
    featureMap.clear();
    loadDefaults();
}

#include <string>
#include <unordered_map>

class FeatureToggle {
public:
    FeatureToggle();
    
    void setEnabled(const std::string& feature, bool state);
    bool isEnabled(const std::string& feature) const;
    void reset();

private:
    std::unordered_map<std::string, bool> featureMap;
    void loadDefaults();
};

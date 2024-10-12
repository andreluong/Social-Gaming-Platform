#include "gtest/gtest.h"
#include "featureToggle.h"

using namespace testing;

class FeatureToggleTest : public::testing::Test {
protected:
    FeatureToggle featureToggle;

    void SetUp() override {
        featureToggle.reset();
    }
};

TEST_F(FeatureToggleTest, loadDefaultsOn) {
    auto defaultOnBool = featureToggle.isEnabled("defaulton");
    bool expected = true;
    EXPECT_EQ(expected, defaultOnBool);
}

TEST_F(FeatureToggleTest, loadDefaultsOff) {
    auto defaultOffBool = featureToggle.isEnabled("defaultoff");
    bool expected = false;
    EXPECT_EQ(expected, defaultOffBool);
}

TEST_F(FeatureToggleTest, loadDefaultsNonExistent) {
    auto nonExistentBool = featureToggle.isEnabled("nonexistent");
    bool expected = false;
    EXPECT_EQ(expected, nonExistentBool);
}

TEST_F(FeatureToggleTest, setEnabledOn) {
    auto featureXBoolBefore = featureToggle.isEnabled("featureX");
    auto expectedFeatureXBoolBefore = false;
    EXPECT_EQ(expectedFeatureXBoolBefore, featureXBoolBefore);

    featureToggle.setEnabled("featureX", true);
    auto featureXBoolAfter = featureToggle.isEnabled("featureX");
    auto expectedFeatureXBoolAfter = true;
    EXPECT_EQ(expectedFeatureXBoolAfter, featureXBoolAfter);
}

TEST_F(FeatureToggleTest, setEnabledOff) {
    featureToggle.setEnabled("featureY", true);
    auto featureYBoolBefore = featureToggle.isEnabled("featureY");
    auto expectedFeatureYBoolBefore = true;
    EXPECT_EQ(expectedFeatureYBoolBefore, featureYBoolBefore);

    featureToggle.setEnabled("featureY", false);
    auto featureYBoolAfter = featureToggle.isEnabled("featureY");
    auto expectedFeatureYBoolAfter = false;
    EXPECT_EQ(expectedFeatureYBoolAfter, featureYBoolAfter);
}

TEST_F(FeatureToggleTest, clear) {
    auto featureZBoolOld = featureToggle.isEnabled("featureZ");
    auto defaultOnBoolOld = featureToggle.isEnabled("defaulton");
    auto defaultOffBoolOld = featureToggle.isEnabled("defaultoff");

    auto expectedFeatureZBool = false;
    auto expectedDefaultOnBool = true;
    auto expectedDefaultOffBool = false;

    EXPECT_EQ(expectedFeatureZBool, featureZBoolOld);
    EXPECT_EQ(expectedDefaultOnBool, defaultOnBoolOld);
    EXPECT_EQ(expectedDefaultOffBool, defaultOffBoolOld);

    featureToggle.setEnabled("featureZ", !expectedFeatureZBool);
    featureToggle.setEnabled("defaulton", !expectedDefaultOnBool);
    featureToggle.setEnabled("defaultoff", !expectedDefaultOffBool);

    auto featureZBoolBeforeReset = featureToggle.isEnabled("featureZ");
    auto defaultOnBoolBeforeReset = featureToggle.isEnabled("defaulton");
    auto defaultOffBoolBeforeReset = featureToggle.isEnabled("defaultoff");

    EXPECT_EQ(true, featureZBoolBeforeReset);
    EXPECT_EQ(false, defaultOnBoolBeforeReset);
    EXPECT_EQ(true, defaultOffBoolBeforeReset);

    featureToggle.reset();

    auto featureZBoolAfterReset = featureToggle.isEnabled("featureZ");
    auto defaultOnBoolAfterReset = featureToggle.isEnabled("defaulton");
    auto defaultOffBoolAfterReset = featureToggle.isEnabled("defaultoff");

    EXPECT_EQ(expectedFeatureZBool, featureZBoolAfterReset);
    EXPECT_EQ(expectedDefaultOnBool, defaultOnBoolAfterReset);
    EXPECT_EQ(expectedDefaultOffBool, defaultOffBoolAfterReset);
}

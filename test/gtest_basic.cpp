
#include <gtest/gtest.h>
#include <string>
#include "scope_map.h"

// Template defaults (TD) default value get behavior
TEST(Basic, TD_GetDefaultValue)
{
    ScopeMap<> sm("n/a");
    EXPECT_EQ(sm["foo"], "n/a");
    EXPECT_EQ(sm.get("bar"), "n/a");
}

TEST(Basic, TDKey_GetDefaultValue)
{
    ScopeMap<unsigned int> sm(17);
    EXPECT_EQ(sm.get("bar"), 17);
    EXPECT_EQ(sm["foo"], 17);
}

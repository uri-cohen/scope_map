
#include <gtest/gtest.h>
#include <string>
#include "scope_map.h"

// Template defaults (TD) default value get behavior
TEST(Basic, TD_GetDefaultValue)
{
    ScopeMap<> sm("n/a");
    EXPECT_EQ(sm.get("bar"), "n/a");
}

TEST(Basic, TDKey_GetDefaultValue)
{
    ScopeMap<unsigned int> sm(17);
    EXPECT_EQ(sm.get("bar"), 17);
}

TEST(Basic, TD_SetGet)
{
    ScopeMap<> sm("n/a");
    EXPECT_EQ(sm.get("bar"), "n/a");
    sm.set("foo", "hello");
    EXPECT_EQ(sm.get("bar"), "n/a");
    sm.set("bar", "world");
    EXPECT_EQ(sm.get("bar"), "world");
}

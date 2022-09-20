#include "mclexer/lexer.h"
#include <gtest/gtest.h>

using namespace lexer;

TEST(LexerTest, BasicAssertions) {
    mc_lexer my_lexer;
    EXPECT_EQ(my_lexer.answer(), 41);
}

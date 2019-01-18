#include <iostream>

#include <gtest/gtest.h>

class ConceptFixture: public testing::Test
{

};

TEST_F(ConceptFixture, test_eq) {
    EXPECT_EQ(0, 0);
}

TEST_F(ConceptFixture, test_eq2) {
    EXPECT_EQ(1, 1);
}

//int main (int argc, char* argv[])
//{
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
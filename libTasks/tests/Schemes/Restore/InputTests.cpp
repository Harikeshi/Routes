#include <Task/Exceptions/ValidationException.hpp>
#include <Task/Schemes/Restore/Input.hpp>

#include <gtest/gtest.h>

using InputRestore = Schemes::Restore::Input;

class InputRestoreTest : public ::testing::Test
{
protected:
    InputRestore input;
    nlohmann::json jsonObject;

    void SetUp() override
    {
        jsonObject = {
            {"Tpk", {0, 0}},
            {"Tbek_0", {0, 0}},
            {"Tbek_exit", {0, 0}},

            {"Dpgas", 0},
            {"Dbgas", 0},
            {"Kt_deg", 0},
            {"Kbek_deg", 0},
            {"Ut", 0},
            {"Ukp", 0},
            {"Ukph", 0},
            {"Ukp_r", 0}};
    }

    void
    TearDown() override
    {
        // Код для очистки после каждого теста
    }
};

TEST_F(InputRestoreTest, fromJsonValidInput)
{
    // std::cout << jsonObject.dump(4);

    EXPECT_NO_THROW(input.fromJson(jsonObject));

    // EXPECT_EQ(input.Tpk, Tpk);
    // EXPECT_EQ(input.Tbek_0, Tbek_0);
    // EXPECT_EQ(input.Tbek_exit, Tbek_exit);

    // EXPECT_EQ(input.Dpgas, 0);
    // EXPECT_EQ(input.Dbgas, 0);
    // EXPECT_EQ(input.Kt_deg, 0);
    // EXPECT_EQ(input.Kbek_deg, 0);
    // EXPECT_EQ(input.Ut, 0);
    // EXPECT_EQ(input.Ukp, 0);
    // EXPECT_EQ(input.Ukph, 0);
    // EXPECT_EQ(input.Ukp_r, 0);
}

TEST_F(InputRestoreTest, fromJsonMissingField)
{
    jsonObject.erase("Tbek_exit");
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputRestoreTest, fromJsonNotDigitField)
{
    jsonObject["Ukph"] = "null";
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputRestoreTest, fromJsonEmptyField)
{
    jsonObject["Ukph"] = "";
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

TEST_F(InputRestoreTest, fromJsonInvalidValueField)
{
    jsonObject["Ukp"] = 200;
    EXPECT_THROW(input.fromJson(jsonObject), Exceptions::ValidationFailure);
}

#include <iostream>
#include <Task/Schemes/Search/AtBorder/Utilities.hpp>
#include <gtest/gtest.h>

using namespace Schemes::Search::AtBorder;


TEST (AtBorderUtils, calculateAParameter)
{
    EXPECT_NEAR(calculateAParameter(1), 0, ACCURACY_DOUBLE);
    EXPECT_NEAR(calculateAParameter(1.5), 0.6708203932499369, ACCURACY_DOUBLE);
    EXPECT_THROW(calculateAParameter(0.9), std::invalid_argument);
}


TEST (AtBorderUtils, relativePatrollingParameters)
{
    PatrollingParameters relativeParameters;

    relativeParameters = relativePatrollingParameters(1.5, "linear");
    EXPECT_NEAR(relativeParameters.patrollingLength, 0.9, ACCURACY_DOUBLE);
    EXPECT_NEAR(relativeParameters.lengthDifference, 1.6, ACCURACY_DOUBLE);

    relativeParameters = relativePatrollingParameters(50, "linear");
    EXPECT_NEAR(relativeParameters.patrollingLength, 49.96004793608947, ACCURACY_DOUBLE);
    EXPECT_NEAR(relativeParameters.lengthDifference, 0.07993607669774315, ACCURACY_DOUBLE);

    relativeParameters = relativePatrollingParameters(0.1, "linear");
    EXPECT_NEAR(relativeParameters.patrollingLength, 0.004993761694389224, ACCURACY_DOUBLE);
    EXPECT_NEAR(relativeParameters.lengthDifference, 1.9975046777556893, ACCURACY_DOUBLE);


    relativeParameters = relativePatrollingParameters(1.5, "cross");
    EXPECT_NEAR(relativeParameters.patrollingLength, 0.7474093186836597, ACCURACY_DOUBLE);
    EXPECT_NEAR(relativeParameters.lengthDifference, 0.8304547985373997, ACCURACY_DOUBLE);

    relativeParameters = relativePatrollingParameters(50, "cross");
    EXPECT_NEAR(relativeParameters.patrollingLength, 97.99920816959698, ACCURACY_DOUBLE);
    EXPECT_NEAR(relativeParameters.lengthDifference, 0.02039983516999774, ACCURACY_DOUBLE);

    EXPECT_THROW(relativePatrollingParameters(0.1, "cross"), std::invalid_argument);
}

/*
TEST(AtBorderUtils, calculateLengths)
{
    std::pair<double, double> lengths;

    lengths = calculateLengths(9, 6, 3, "linear");
    EXPECT_NEAR(lengths.first, 2.7, EPS);
    EXPECT_NEAR(lengths.second, 7.5, EPS);

    lengths = calculateLengths(6, 9, 3, "linear");
    EXPECT_NEAR(lengths.first, 0.6324555320336758, EPS);
    EXPECT_NEAR(lengths.second, 6.324555320336759, EPS);

    lengths = calculateLengths(1, 1, 4, "cross");
    EXPECT_NEAR(lengths.first, 0, EPS);
    EXPECT_NEAR(lengths.second, 8, EPS);

    lengths = calculateLengths(15, 10, 4, "cross");
    EXPECT_NEAR(lengths.first, 2.9896372747346387, EPS);
    EXPECT_NEAR(lengths.second, 9.633275663033833, EPS);

    lengths = calculateLengths(18, 9, 2, "cross");
    EXPECT_NEAR(lengths.first, 3.491486243775878, EPS);
    EXPECT_NEAR(lengths.second, 6.110100926607787, EPS);

}
*/

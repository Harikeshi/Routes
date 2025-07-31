// #include "Project/Database/DatabaseRepository.hpp"
// #include "mocks/MockConnection.hpp"
// #include "mocks/MockTransaction.hpp"
// #include <gtest/gtest.h>

// using namespace testing;

// class DatabaseRepositoryTest : public Test {
// protected:
//     std::shared_ptr<MockConnection> mockConn;
//     std::unique_ptr<MockTransaction> mockTxn;
//     std::unique_ptr<DatabaseRepository> repo;

//     void SetUp() override {
//         mockConn = std::make_shared<MockConnection>();
//         mockTxn = std::make_unique<MockTransaction>(*mockConn);
//         repo = std::make_unique<DatabaseRepository>(mockConn);
//     }
// };

// TEST_F(DatabaseRepositoryTest, UpdateShip_Success) {
//     // Подготовка
//     EXPECT_CALL(*mockConn, is_open()).WillOnce(Return(true));
//     EXPECT_CALL(*mockConn, begin()).WillOnce(Return(ByMove(std::move(mockTxn))));

//     pqxx::result expectedResult;
//     expectedResult.emplace_back().emplace_back(1); // Возвращаем ID = 1

//     EXPECT_CALL(*mockTxn, exec_params(_, _))
//         .WillOnce(Return(expectedResult));
//     EXPECT_CALL(*mockTxn, commit());

//     // Выполнение
//     auto result = repo->updateShip(1, 10, 20);

//     // Проверка
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(*result, 1);
// }

// TEST_F(DatabaseRepositoryTest, UpdateShip_ConnectionClosed) {
//     EXPECT_CALL(*mockConn, is_open()).WillOnce(Return(false));

//     auto result = repo->updateShip(1, 10, 20);
//     EXPECT_FALSE(result.has_value());
// }

// TEST_F(DatabaseRepositoryTest, UpdateShip_TransactionFailed) {
//     EXPECT_CALL(*mockConn, is_open()).WillOnce(Return(true));
//     EXPECT_CALL(*mockConn, begin()).WillOnce(Return(ByMove(std::move(mockTxn))));
//     EXPECT_CALL(*mockTxn, exec_params(_, _))
//         .WillOnce(Throw(pqxx::sql_error("Test error")));
//     EXPECT_CALL(*mockTxn, abort());

//     auto result = repo->updateShip(1, 10, 20);
//     EXPECT_FALSE(result.has_value());
// }

// TEST_F(DatabaseRepositoryTest, UpdateShip_NullResult) {
//     EXPECT_CALL(*mockConn, is_open()).WillOnce(Return(true));
//     EXPECT_CALL(*mockConn, begin()).WillOnce(Return(ByMove(std::move(mockTxn))));

//     pqxx::result emptyResult;
//     EXPECT_CALL(*mockTxn, exec_params(_, _))
//         .WillOnce(Return(emptyResult));
//     EXPECT_CALL(*mockTxn, commit());

//     auto result = repo->updateShip(1, 10, 20);
//     EXPECT_FALSE(result.has_value());
// }

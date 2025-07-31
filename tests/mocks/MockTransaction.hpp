// #pragma once
// #include <pqxx/pqxx>
// #include <gmock/gmock.h>

// class MockTransaction : public pqxx::transaction<> {
// public:
//     explicit MockTransaction(pqxx::connection& c) : pqxx::transaction<>(c) {}

//     MOCK_METHOD(pqxx::result, exec, (const std::string&), (override));
//     MOCK_METHOD(pqxx::result, exec_params,
//         (const std::string&, const std::vector<std::variant<int, std::string, std::nullptr_t>>&),
//         (override));
//     MOCK_METHOD(void, commit, (), (override));
//     MOCK_METHOD(void, abort, (), (override));
// };

// #ifndef SETUP_HPP
// #define SETUP_HPP

// #include <Project/Database/DataAccessObjects/EventDAO.hpp>
// #include <Project/Database/DataAccessObjects/RequestDAO.hpp>
// #include <Project/Database/DataAccessObjects/ShipDAO.hpp>
// #include <Project/Database/DatabaseConnection.hpp>

// #include <iostream>
// #include <nlohmann/json.hpp>

// class Setup
// {
//     static int testExecute()
//     {
//         try
//         {
//             // Создаем подключение
//             const auto connection = DatabaseConnection::createConnection();

//             // Инициализируем DAO объекты
//             const ShipDAO shipDao(connection);
//             const RequestDAO requestDao(connection);
//             const EventDAO eventDao(connection);

//             // Создаем корабль
//             const int shipId = shipDao.update(10, 5);
//             std::cout << "Created ship with ID: " << shipId << std::endl;

//             // Создаем запрос
//             const int requestId = requestDao.update(
//                 123456789, "Target1", "North", "Border1", shipId);
//             std::cout << "Created request with ID: " << requestId << std::endl;

//             // Создаем событие
//             const nlohmann::json eventData = {
//                 {"key", "value"},
//                 {"number", 42}};
//             const int eventId = eventDao.update(
//                 "user123", "Test message", eventData, requestId);
//             std::cout << "Created event with ID: " << eventId << std::endl;
//         }
//         catch (const std::exception& e)
//         {
//             std::cerr << "Error: " << e.what() << std::endl;
//             return 1;
//         }
//         return 0;
//     }
// };
// #endif

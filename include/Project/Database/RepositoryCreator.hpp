#pragma once

#include "PostgreSQLRepository.hpp"
#include "JsonRepository.hpp"
#include <QString>

enum class RepositoryType {
    PostgreSQL,
    JsonFile
};

class RepositoryCreator {
public:
    static std::unique_ptr<Database::IRepository> create(
            RepositoryType type,
            const QString &connectionString = "",
            const QString &jsonBasePath = ""
    ) {
        switch (type) {
            case RepositoryType::PostgreSQL:
                return std::make_unique<Database::PostgreSQLRepository>(connectionString);
            case RepositoryType::JsonFile:
                return std::make_unique<Database::JsonRepository>(jsonBasePath);
            default:
                throw std::invalid_argument("Unknown repository type");
        }
    }
};
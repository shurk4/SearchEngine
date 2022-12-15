#pragma once

#include <iostream>
#include <exception>
#include <fstream>

#include "nlohmann/json.hpp"

class TestFilesExceptions : public std::exception
{
    const char* ex;
    const char* what() const noexcept override
    {
        return ex;
    }

public:
    explicit TestFilesExceptions(const char* text) : ex(text){}
};

using json = nlohmann::json;

class TestFiles
{
    static nlohmann::json config;
    static nlohmann::json requests;
    static std::vector<std::vector<std::string>> docs;

public:
    TestFiles() = delete;
    TestFiles(const TestFiles&) = delete;
    void operator = ( const TestFiles& ) = delete;
// метод создаёт файл конфигурации config.json
    static void createConfig (int in);

// метод создаёт файл запросов request.json
    static void createRequests (int in);

// метод создаёт тестовые файлы
    static void createFiles(int in);
};
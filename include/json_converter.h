#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <exception>
#include <sstream>
#include "nlohmann/json.hpp"

class ConverterExceptions : public std::exception
{
    const char* ex;
    const char* what() const noexcept override // const - метод не меняет состояние объекта класса, noexcept - метод гарантированно не генерирует исключений
    {
        return ex;
    }

public:
    explicit ConverterExceptions(const char* text) : ex(text){}
};

using json = nlohmann::json;
//--------------------------------------------------------------------------------------------

/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON {
    // параметры из config.json
    std::string name;
    std::string version;
    int maxResponses = 5;
    std::vector<std::string> files;
    // запросы из requests.json
    std::vector<std::string> requests;

public:
    ConverterJSON();

//    ~ConverterJSON();

    std::string GetName() const;
    std::string GetVersion() const;

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
    std::vector<std::string> GetTextDocuments();

/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
    int GetResponsesLimit() const;

/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests() const;

/**
* Положить в файл answers.json результаты поисковых запросов
*/
    static void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
};

//--------------------------------------------------------------------------------------------

// функция создаёт файл конфигурации config.json
void createConfig (const json &in);

// функция создаёт файл запросов request.json
void createRequests (const json &in);

// функция создаёт тестовые файлы
std::vector<std::string> createFiles(const std::vector<std::string> &in);
void createTest1();
void createTest2();

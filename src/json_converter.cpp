#include "json_converter.h"

using json = nlohmann::json;

// конструктор
ConverterJSON::ConverterJSON()
{
    json tempJson;
    std::stringstream tempStream;
    std::string str;

    std::ifstream file("config.json");
    if(!file.is_open())
    {
        throw ConverterExceptions("config file is missing.");
    }
    if (!nlohmann::json::accept(file))
    {
        throw ConverterExceptions("config file have wrong format");
    }
    file.seekg(0, std::ios_base::beg);

    file >> tempJson;
    file.close();

    auto it = tempJson.find("config");
    if (it == tempJson.end())
    {
        throw ConverterExceptions("config file is empty\n");
    }

    // параметры из config.json
    name = tempJson["config"]["name"];
    version = tempJson["config"]["version"];
    maxResponses = tempJson["config"]["max_responses"];

    for(auto &it2 : tempJson["files"])
    {
        files.push_back(it2);
    }
    tempJson.clear();

    // запросы из requests.json
    file.open("requests.json");
    if(!file.is_open())
    {
        throw ConverterExceptions("Couldn't open the request.json file!");
    }
    file >> tempJson;
    file.close();

    for(auto &i : tempJson["requests"])
    {
        requests.push_back(i);
    }
}

std::string ConverterJSON::GetName() const
{
    return name;
}

std::string ConverterJSON::GetVersion() const
{
    return version;
}

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
std::vector<std::string> ConverterJSON::GetTextDocuments() const
{
    std::vector<std::string> result;

    for(auto &i : files)
    {
        std::ifstream file(i);
        std::string text;

        if(!file.is_open())
        {
            throw ConverterExceptions("Couldn't open document");
        }

        while(!file.eof())
        {
            std::string word;
            file >> word;
            text += word;
            if(!file.eof())
            {
                text += " ";
            }
        }
        file.close();

        result.push_back(text);
    }
    return result;
}

/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
*/
int ConverterJSON::GetResponsesLimit() const
{
    return maxResponses;
}

/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
std::vector<std::string> ConverterJSON::GetRequests() const
{
    return requests;
}

/**
* Положить в файл answers.json результаты поисковых запросов
*/
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
    nlohmann::json js;

    js["answers"];

    for(int i = 0; i < answers.size(); i++)
    {
        std::stringstream tempStream;
        tempStream << "request" << std::setfill('0') << std::setw(3) << i + 1;

        js["answers"][tempStream.str()];

        for(int j = 0; j < answers[i].size(); j++)
        {
            if(j == 0)
            {
                if(answers[i][j].second == 0)
                {
                    js["answers"][tempStream.str()]["result"] = false;
                    break;
                }
                else
                {
                    js["answers"][tempStream.str()]["result"] = true;
                }
            }
            js["answers"][tempStream.str()]["relevance"][j] = {{"docid" , answers[i][j].first} , {"rank", answers[i][j].second}};
        }
    }

    std::ofstream file("answer.json");
    if(!file.is_open())
    {
        throw ConverterExceptions("Couldn't create answer.json file");
    }

    file << std::setw(4) << js;
    file.close();
}
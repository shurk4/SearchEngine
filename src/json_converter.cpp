#include "json_converter.h"

using json = nlohmann::json;

// конструктор
ConverterJSON::ConverterJSON()
{
    json tempJson;
    std::stringstream tempStream;
    std::string str;

    std::ifstream file("../config.json"); // добавить проверку наличия/открытия файла !!! //---доделать
    if(!file.is_open())
    {
        throw ConverterExceptions("config file is missing.\n");
    }
    if (!nlohmann::json::accept(file))
    {
        throw ConverterExceptions("config file have wrong format\n");
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

    for(auto &it : tempJson["files"])
    {
        files.push_back(it);
    }
    tempJson.clear();

    // запросы из requests.json
    file.open("../requests.json");
    if(!file.is_open())
    {
        throw ConverterExceptions("Could't open the request.json file!");
    }
    tempJson << file;
    file.close();

    for(auto &it : tempJson["requests"])
    {
        requests.push_back(it);
    }
}

ConverterJSON::~ConverterJSON()
{
    files.empty();
    requests.empty();
}

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    std::vector<std::string> result;

    for(auto &i : files)
    {
        std::ifstream file(i);
        std::string text;

        if(!file.is_open())
        {
            throw ConverterExceptions("Could't open docement");
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
int ConverterJSON::GetResponsesLimit()
{
    return maxResponses;
}

/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
std::vector<std::string> ConverterJSON::GetRequests()
{
    return requests;
}

/**
* Положить в файл answers.json результаты поисковых запросов
*/
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) //---доделать
{
    std::stringstream tempStream;
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

    std::ofstream file("../result.json");
    if(!file.is_open())
    {
        throw ConverterExceptions("Could't create result.json file");
    }

    file << std::setw(4) << js;
    file.close();
}

//----------------------------------------------------------------------------------------------------------------------

void ConverterJSON::printConfig()
{
    std::cout << "\n------------ CONFIG ------------" << std::endl;
    std::cout << "\nName: " << name << "\nversion: " << version << "\nmaxResponses: " << maxResponses << std::endl;
    std::cout << "\nFiles: " << std::endl;
    for(auto &i : files)
    {
        std::cout << "\t" << i << std::endl;
    }
    std::cout << "------------ CONFIG ------------" << std::endl;
}

void ConverterJSON::printRequests()
{
    std::cout << "\n------------ REQUESTS ------------" << std::endl;
    for(auto &i : requests)
    {
        std::cout << "\t" << i << std::endl;
    }
    std::cout << "------------ REQUESTS ------------" << std::endl;
}

void createConfig ()
{
    std::cout << "Create config" << std::endl;
    json tempJson = {
            {"config" ,{
                               {"name", "shuriksSearchEngine"},
                               {"version", "0.1"},
                               {"max_responses", 5}
                       }},
            {"files" ,{
                               "../../resources/file001.txt",
                               "../../resources/file002.txt",
                               "../../resources/file003.txt",
                               "../../resources/file004.txt"
                       }}
    };

    std::ofstream file ("../config.json");

    if(!file.is_open())
    {
        throw ConverterExceptions("Could't create config.json file");
    }

    file << std::setw(4) << tempJson;
    std::cout << "Config created" << std::endl;
    file.close();
}

void createRequests ()
{
    std::cout << "Created requests" << std::endl;
    json tempJson = {
            {"requests" ,{
                "milk",
                "water",
                "cappuchino"
            }}
    };

    std::ofstream file ("../requests.json");

    if(!file.is_open())
    {
        throw ConverterExceptions("Could't create requests.json file");
    }

    file << std::setw(4) << tempJson;
    std::cout << "Requests created" << std::endl;
    file.close();
}
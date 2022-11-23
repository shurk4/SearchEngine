#include "json_converter.h"

using json = nlohmann::json;

// конструктор
ConverterJSON::ConverterJSON()
{
    json tempJson;
    std::stringstream tempStream;
    std::string str;

    std::ifstream file("config.json"); // добавить проверку наличия/открытия файла !!! //---доделать
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

//ConverterJSON::~ConverterJSON()
//{
//    files.empty();
//    requests.empty();
//}

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
std::vector<std::string> ConverterJSON::GetTextDocuments()
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
std::vector<std::string> ConverterJSON::GetRequests()
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

//----------------------------------------------------------------------------------------------------------------------

void createConfig (const json &in)
{
    std::cout << "Create config" << std::endl;

    std::ofstream file ("config.json");

    if(!file.is_open())
    {
        throw ConverterExceptions("Couldn't create config.json file");
    }

    file << std::setw(4) << in;
    std::cout << "Config created" << std::endl;
    file.close();
}

void createRequests (const json &in)
{
    std::cout << "Created requests" << std::endl;

    std::ofstream file ("requests.json");

    if(!file.is_open())
    {
        throw ConverterExceptions("Couldn't create requests.json file");
    }

    file << std::setw(4) << in;
    std::cout << "Requests created" << std::endl;
    file.close();
}

std::vector<std::string> createFiles(const std::vector<std::string> &in)
{
    std::vector<std::string> result;
    for(int i = 0; i < in.size(); i++)
    {
        std::stringstream fileName;
        fileName << "file" << std::setfill('0') << std::setw(3) << i + 1 << ".txt";
        std::ofstream  file(fileName.str());
        file << in[i];
        file.close();
        result.push_back(fileName.str());
    }
    return result;
}

void createTest1()
{
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const std::vector<std::string> request = {"milk water", "sugar"};
    std::vector<std::string> tempFiles = createFiles(docs);

    json tempRequests = {
            {"requests" ,
             {"milk water",
              "sugar"
             }}
    };

    createRequests(tempRequests);


    json tempConfig = {
            {"config" ,{
                               {"name", "shuriksSearchEngine"},
                               {"version", "0.1"},
                               {"max_responses", 5}
                       }},
            {"files", {}}
    };

    std::cout << "temp config:\n" << std::setw(4) << tempConfig << "\n";

    json tempJson;
    std::stringstream temp;
    for(auto i : tempFiles)
    {
        tempConfig["files"].push_back(i);
    }

    std::cout << "temp config:\n" << std::setw(4) << tempConfig << "\n";

    createConfig(tempConfig);
}

void createTest2()
{
    const std::vector<std::string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
    };

    std::vector<std::string> tempFiles = createFiles(docs);

    json tempRequests = {
            {"requests" ,
             {"moscow is the capital of russia"
             }}
    };

    createRequests(tempRequests);


    json tempConfig = {
            {"config" ,{
                               {"name", "shuriksSearchEngine"},
                               {"version", "0.1"},
                               {"max_responses", 5}
                       }},
            {"files", {}}
    };

    std::cout << "temp config:\n" << std::setw(4) << tempConfig << "\n";

    json tempJson;
    std::stringstream temp;
    for(auto i : tempFiles)
    {
        tempConfig["files"].push_back(i);
    }

    std::cout << "temp config:\n" << std::setw(4) << tempConfig << "\n";

    createConfig(tempConfig);
}
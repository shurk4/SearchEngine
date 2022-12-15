#include "testFiles.h"

void TestFiles::createConfig (int in)
{
    std::cout << "Create config" << std::endl;

    std::ofstream file ("config.json");

    if(!file.is_open())
    {
        throw TestFilesExceptions("Couldn't create config.json file");
    }

    file << std::setw(4) << config[in];
    std::cout << "Config created" << std::endl;
    file.close();
}

void TestFiles::createRequests (int in)
{
    std::cout << "Create requests" << std::endl;

    std::ofstream file ("requests.json");

    if(!file.is_open())
    {
        throw TestFilesExceptions("Couldn't create requests.json file");
    }

    file << std::setw(4) << requests[in];
    std::cout << "Requests created\n";
    file.close();
}

void TestFiles::createFiles(int in)
{
    std::cout << "Create files\n";

    for(int i = 0; i < docs[in].size(); i++)
    {
        std::ofstream  file(config[in]["files"][i].get<std::string>());
        file << docs[in][i];
        file.close();
    }

    std::cout << "Files created\n";
}

nlohmann::json TestFiles::config = {
        {{"config" ,{
                            {"name", "SearchEngine"},
                            {"version", "0.1"},
                            {"max_responses", 5}
                    }},
                {"files",
                            {"file001.txt",
                             "file002.txt",
                             "file003.txt",
                             "file004.txt"}}},
        {{"config" ,{
                            {"name", "SearchEngine"},
                            {"version", "0.1"},
                            {"max_responses", 5}
                    }},
                {"files",
                            {"file001.txt",
                             "file002.txt",
                             "file003.txt",
                             "file004.txt",
                             "file005.txt",
                             "file006.txt",
                             "file007.txt",
                             "file008.txt",
                             "file009.txt",
                             "file010.txt",
                             "file011.txt",
                             "file012.txt",
                             "file013.txt",
                             "file014.txt",
                             "file015.txt",
                             "file016.txt",
                             "file017.txt",
                             "file018.txt",
                             "file019.txt",
                             "file020.txt",
                             "file021.txt",
                             "file022.txt"}}}
};

nlohmann::json TestFiles::requests = {
        {{"requests" ,
                 {"milk water",
                  "sugar"}}},
        {{"requests" ,
                 {"moscow is the capital of russia"}}}
};

std::vector<std::vector<std::string>> TestFiles::docs = {
        {
                "milk milk milk milk water water water",
                "milk water water",
                "milk milk milk milk milk water water water water water",
                "americano cappuccino"
        },
        {
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
        }
};
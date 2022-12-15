#include "jsonConverter.h"
#include "invertedIndex.h"
#include "searchServer.h"
#include "testFiles.h"

std::vector<std::vector<std::pair<int, float>>> resultToConverter(const std::vector<std::vector<RelativeIndex>> &relativeIndex)
{
    std::vector<std::vector<std::pair<int, float>>> result(relativeIndex.size());
    for (int i = 0; i < relativeIndex.size(); i++) {
        for (int j = 0; j < relativeIndex[i].size(); j++) {
            result[i].push_back(std::make_pair(relativeIndex[i][j].docId, relativeIndex[i][j].rank));
        }
    }
    return result;
}

void hello()
{
    std::cout << "------=======        Welcome to Search Engine        =======------\n";
    std::cout << "Commands:\n"
                 "c - For automatic creation of test configuration and request files\n"
                 "s - To start engine\n"
                 "q - To quit\n"
                 "------======================================================------\n";
}

void createTestFiles(int in)
{
    try
    {
        TestFiles::createConfig(in);
        TestFiles::createRequests(in);
        TestFiles::createFiles(in);
    }
    catch(std::exception &ex)
    {
        std::cerr << "Create file exception:\n" << ex.what() << "\n";
    }
}

void startEngine()
{
    try
    {
        ConverterJSON converter;

        std::vector<std::string> documents = converter.GetTextDocuments();
        InvertedIndex index;

        index.UpdateDocumentBase(documents);

        SearchServer searchServer(index);
        searchServer.setMaxResponse(converter.GetResponsesLimit());

        std::vector<std::vector<RelativeIndex>> relativeIndex = searchServer.search(converter.GetRequests());

        ConverterJSON::putAnswers(resultToConverter(relativeIndex));

        std::cout << "Search completed! " << "\n";
    }
    catch(std::exception &ex)
    {
        std::cerr << "Exception:\n" << ex.what() << "\n";
    }
}

char input()
{
    std::string str;
    while(str.size() != 1)
    {
        std::cin >> str;
        if(str.size() != 1)
        {
            std::cout << "Wrong input! Try again:\n";
        }
    }
    return (char)std::tolower(str[0]);
}

void mainMenu()
{
    while(true)
    {
        std::cout << "Enter the command: ";
        char in = input();
        if(in == 'c')
        {
            while(in != '1' && in != '2')
            {
                std::cout << "Choice the test 1 or 2: ";
                in = input();
                if(in != '1' && in != '2'){
                    std::cout << "Wrong input! Try again:\n";
                }
            }
            createTestFiles((in - '0') - 1);
            std::cout << "\n";
        }
        else if(in == 's')
        {
            startEngine();
        }
        else if(in == 'q')
        {
            std::cout << "Bye!\n";
            break;
        }
        else
        {
            std::cout << "Wrong input! Try again:\n";
        }
    }
}

int main() {

    hello();
    mainMenu();

    return 0;
}

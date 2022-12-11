#include "json_converter.h"
#include "invertedIndex.h"
#include "searchServer.h"
#include "testFiles.h"

std::vector<std::vector<std::pair<int, float>>> resultToConverter(const std::vector<std::vector<RelativeIndex>> &relativeIndex)
{
    std::vector<std::vector<std::pair<int, float>>> result(relativeIndex.size());
    for (int i = 0; i < relativeIndex.size(); i++) {
        for (int j = 0; j < relativeIndex[i].size(); j++) {
            result[i].push_back(std::make_pair(relativeIndex[i][j].doc_id, relativeIndex[i][j].rank));
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

        std::cout << "Welcome to Search Engine v. " << converter.GetVersion() << "\n";

        std::vector<std::string> documents = converter.GetTextDocuments();
        InvertedIndex index;

        index.UpdateDocumentBase(documents);

        SearchServer searchServer(index);
        searchServer.setMaxResponse(converter.GetResponsesLimit());

        std::vector<std::vector<RelativeIndex>> relativeIndex = searchServer.search(converter.GetRequests());

        ConverterJSON::putAnswers(resultToConverter(relativeIndex));

        std::cout << "Search completed! " << converter.GetName() << " " << converter.GetVersion() << "\n";
    }
    catch(std::exception &ex)
    {
        std::cerr << "Exception:\n" << ex.what() << "\n";
    }
}

void mainMenu()
{
    char input = ' ';
    while((char)std::tolower(input) != 'q')
    {
        std::cout << "Enter the command: ";
        std::cin >> input;
        if(input == 'c' || input == 'C')
        {
            while(input != '1' && input != '2')
            {
                std::cout << "Choice the test 1 or 2: ";
                std::cin >> input;
                if(input != '1' && input != '2'){
                    std::cout << "Wrong input\n";
                }
            }
            createTestFiles((input - '0') - 1);
            std::cout << "\n";
        }
        else if((char)std::tolower(input) == 'q')
        {
            std::cout << "Bye!\n";
        }
        else if((char)std::tolower(input) == 's')
        {
            startEngine();
        }
    }
}

int main() {

    hello();
    mainMenu();

    return 0;
}

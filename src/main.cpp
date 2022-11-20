#include <iostream>
#include <thread>
#include <vector>

#include "json_converter.h"
#include "invertedIndex.h"
#include "searchServer.h"

std::vector<std::vector<std::pair<int, float>>> resultToConverter(const std::vector<std::vector<RelativeIndex>> relativeIndex)
{
    std::vector<std::vector<std::pair<int, float>>> result(relativeIndex.size());
    for (int i = 0; i < relativeIndex.size(); i++) {
        for (int j = 0; j < relativeIndex[i].size(); j++) {
            result[i].push_back(std::make_pair(relativeIndex[i][j].doc_id, relativeIndex[i][j].rank));
        }
    }
    return result;
}

int main() {
    std::cout << "------=======        Welcome to Search Engine        =======------\n";
    std::cout << "Commands:\n"
                 "C - For automatic creation of test configuration and request files\n"
                 "S - To start engine\n"
                 "Q - To quit\n"
                 "------======================================================------\n";
    std::string input;
    while(input != "q")
    {
        std::cout << "Enter the command: ";
        std::cin >> input;
        if(input == "c" || input == "C")
        {
            while(input != "1" && input != "2")
            {
                std::cout << "Choise the test 1 or 2: ";
                std::cin >> input;
                if(input != "1" && input != "2"){
                    std::cout << "Wrong input\n";
                }
            }
            if(input == "1")
            {
                createTest1();
            }
            if(input == "2")
            {
                createTest2();
            }
        }
        else if(input == "q" || input == "Q")
        {
            std::cout << "Bye!\n";
        }
        else if(input == "s" || input == "S")
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

                converter.putAnswers(resultToConverter(relativeIndex));

                std::cout << "Search completed!" << converter.GetVersion() << "\n";

            }
            catch(std::exception &ex)
            {
                std::cerr << "Exception:\n" << ex.what() << "\n";
            }
        }
    }


    return 0;
}

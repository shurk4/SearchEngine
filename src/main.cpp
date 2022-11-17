#include <iostream>
#include "json_converter.h"
#include "invertedIndex.h"
#include "searchServer.h"
#include <thread>

#include <vector>
#include <fstream>

int main() {
    createConfig();
    createRequests();

try
{
    ConverterJSON converter;
//
//    converter.printConfig();
//    converter.printRequests();

    std::vector<std::string> documents = converter.GetTextDocuments();
    InvertedIndex index;

    index.UpdateDocumentBase(documents);

    SearchServer searchServer(index);
    searchServer.setMaxResponse(converter.GetResponsesLimit());

    std::vector<std::vector<RelativeIndex>> relativeIndex = searchServer.search(converter.GetRequests());
//----------------------------------------------------------------------------------------------------------------------
// конвертация результата searchServer для json_converter
    std::vector<std::vector<std::pair<int, float>>> resultToConverter(relativeIndex.size());

    for(int i = 0; i < relativeIndex.size(); i++)
    {
        for(int j = 0; j < relativeIndex[i].size(); j++)
        {
            resultToConverter[i].push_back(std::make_pair(relativeIndex[i][j].doc_id, relativeIndex[i][j].rank));
        }
    }
//----------------------------------------------------------------------------------------------------------------------

    converter.putAnswers(resultToConverter);

}
catch(std::exception &ex)
{
    std::cerr << "Exception:\n" << ex.what() << "\n";
}

    return 0;
}

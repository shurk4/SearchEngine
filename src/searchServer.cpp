#include <iomanip>
#include "searchServer.h"

bool symbol(const char &ch)
{
    return (ch < 65 || ch > 90 && ch < 97 || ch > 122);
}

/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
SearchServer::SearchServer(InvertedIndex& idx) : _index(idx) {}

/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
    std::vector<std::vector<RelativeIndex>> result;

    // разделение запроса на слова, поиск по словам, создание релевантного индекса запроса
    for(auto &i : queries_input)
    {
        std::stringstream tempStream(i);
        std::string word;
        std::map<std::string, int> uniqueWords;
        // 1. Разбивает поисковый запрос на отдельные слова
        while(tempStream >> word)
        {
            if(symbol(word[word.size() - 1]))
            {
                word.pop_back();
            }

//        2. Формирует из них список уникальных.
            auto it = uniqueWords.find(word);

            if(it == uniqueWords.end())
            {
                uniqueWords.emplace(word, 0);
            }
            else // если встречаемость в списке запросов
            {
                it->second++;
            }
        }
//        3. Сортирует слова в порядке увеличения частоты встречаемости: от самых
//        редких до самых частых. По возрастанию значения поля count поля
//        freq_dictionary.
        std::multimap<int, std::string> sortedWords;

        for(auto &j : uniqueWords)
        {
            sortedWords.emplace(j.second, j.first);
        }
//        4. По первому, самому редкому слову из списка находит все документы, в которых
//        встречается слово.
//        5. Далее ищет соответствия следующего слова и этого списка документов. Так по
//        каждому следующему слову.
//        6. Если в итоге не осталось ни одного документа, то выводит количество
//        найденных документов, равное 0. В результат ответа записывает false.
        std::map<size_t, float> tempIndex;
        bool wordFound = false;

        for(auto  &it : sortedWords)
        {
            std::vector<Entry> wordEntries = _index.GetWordCount(it.second);

            wordFound = true;

            for (auto &j : wordEntries)
            {
                auto it2 = tempIndex.find(j.doc_id);
                if(it2 == tempIndex.end())
                {
                    tempIndex.emplace(j.doc_id, j.count);
                }
                else
                {
                    it2->second += j.count;
                }
            }
        }
//        7. Если документы найдены, рассчитывает по каждому из них релевантность и
//        выводит её в поле rank в ответе на запрос. Для этого для каждого документа
//        рассчитывается абсолютная релевантность — сумма всех count всех найденных
//        в документе слов из коллекции freq_dictionary, которая делится
        std::vector<RelativeIndex> index;
        RelativeIndex temp;
        float maxIndex = 0;

        if(wordFound)
        {
            for(auto it : tempIndex)
            {
                if(it.second > maxIndex)
                {
                    maxIndex = it.second;
                }
                temp.doc_id = it.first;
                temp.rank = it.second;
                index.push_back(temp);
            }
        }
        else
        {
            index.emplace_back();
        }

        for(auto &i : index)
        {
            i.rank /= maxIndex;
        }

        std::sort(index.begin(), index.end());

        if(index.size() > maxResponse)
        {
            index.erase(index.begin() + maxResponse, index.end());
        }

        result.push_back(index);
    }
    return result;
}

void SearchServer::setMaxResponse(const int &response)
{
    maxResponse = response;
}


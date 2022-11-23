#include "invertedIndex.h"
#include <iostream>

// Конструктор для SearchServer
InvertedIndex::InvertedIndex(InvertedIndex &other)
{
    docs = other.docs;
    freq_dictionary = other.freq_dictionary;
}

// подсчёт слов в документе и добавление результата в freq_dictionary
void InvertedIndex::WordsCount()
{
    while(true)
    {
        docsLock.lock();
        if(docsIter >= docs.size())
        {
            docsLock.unlock();
            break;
        }
        std::stringstream tempStream(docs[docsIter]);
        int currentDocIter = docsIter;
        docsIter++;
        docsLock.unlock();

        std::string word;
        std::map<std::string, std::vector<Entry>> dictionary;

        while(tempStream >> word)
        {
            auto it = dictionary.find(word);
            Entry entry;

            if(dictionary.empty() || it == dictionary.end())
            {
                entry.doc_id = currentDocIter;
                entry.count = 1;

                std::vector<Entry> temp;
                temp.push_back(entry);

                dictionary.emplace(word, temp);
            }
            else
            {
                    it->second[0].count++;
            }
        }


        freq_dictionaryLock.lock();

        if(freq_dictionary.empty())
        {
            freq_dictionary.insert(dictionary.begin(), dictionary.end());
        }
        else
        {
            for(auto it : dictionary)
            {
                auto it2 = freq_dictionary.find(it.first);
                if(it2 == freq_dictionary.end())
                {
                    freq_dictionary.emplace(it.first, it.second);
                }
                else
                {
                    it2->second.push_back(it.second[0]);
                }
            }
        }
        freq_dictionaryLock.unlock();
    }
//    std::cout << "Freq_dictionary size: " << freq_dictionary.size() << "\n";
}

//Составление словаря
void InvertedIndex::CollectDictionary()
{
    freq_dictionary.clear();
    docsIter = 0;
    size_t threadsNum;

    if(std::thread::hardware_concurrency() > 1)
    {
        threadsNum = std::thread::hardware_concurrency() - 1;
    }
    else
    {
        threadsNum = 1;
    }

    std::vector<std::thread> threads(threadsNum);

//    std::cout << "Started CollectDictionary. Basic thread ID: " << std::this_thread::get_id() << "\n";

    for(int i = 0; i < threadsNum; i++)
    {
//        std::cout << "Starting thread " << i + 1 << "\n";

        threads[i] = std::thread([&]()
                                      {
                                          WordsCount();
                                      });

//        threads.push_back(std::thread([&]()
//                                      {
//                                          WordsCount();
//                                      }));
    }

    for(int i = 0; i < threadsNum; i++)
    {
        if(threads[i].joinable()) threads[i].join();
    }

//    std::cout << "!CollectDictionary completed!\n";
}

/**
* Обновить или заполнить базу документов, по которой будем совершать
поиск*
@param texts_input содержимое документов
*/
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
    bool haveChanges = false;
    if(docs.size() != input_docs.size())
    {
        docs.resize(input_docs.size());
        haveChanges = true;
    }

    for(int i = 0; i < input_docs.size(); i++)
    {
        if(docs[i] != input_docs[i])
        {
            docs[i] = input_docs[i];
            haveChanges = true;
        }
    }
    //составление freq_dictionary перенесено в temp и в words count (С изменениями)

    if(haveChanges)
    {
        CollectDictionary();
    }
}
/**
 * * Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
{
    auto it = freq_dictionary.find(word);
    if(it != freq_dictionary.end())
    {
        return it->second;
    }
    else
    {
        std::vector<Entry> empty;
        return empty;
    }
}

void InvertedIndex::freq_dictionary_print()
{
    std::cout << "----=== FREQ DICTIONARY:\n";
    for(auto const &it : freq_dictionary)
    {
        std::cout << "Word: " << it.first << "\n";
        for(auto it2 : it.second)
        {
            std::cout << " doc_id: " << it2.doc_id << " count: " << it2.count << "\n";
        }

    }
}
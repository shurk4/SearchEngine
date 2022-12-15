#include "invertedIndex.h"

// Конструктор для SearchServer
InvertedIndex::InvertedIndex(InvertedIndex &other)
{
    docs = other.docs;
    freqDictionary = other.freqDictionary;
}

// Подсчёт повторяющихся слов в документе
std::map<std::string, std::vector<Entry>> InvertedIndex::WordsEntry(int iter)
{
    std::stringstream tempStream(docs[iter]);

    std::string word;
    std::map<std::string, std::vector<Entry>> result;

    while(tempStream >> word)
    {
        auto it = result.find(word);
        Entry entry;

        if(result.empty() || it == result.end())
        {
            entry.docId = iter;
            entry.count = 1;

            std::vector<Entry> temp;
            temp.push_back(entry);

            result.emplace(word, temp);
        }
        else
        {
            it->second[0].count++;
        }
    }
    return result;
}

// Дополнение частотного словаря
void InvertedIndex::FreqDictionaryAppend(std::map<std::string, std::vector<Entry>> in)
{
    freqDictionaryLock.lock();

    if(freqDictionary.empty())
    {
        freqDictionary.insert(in.begin(), in.end());
    }
    else
    {
        for(auto it : in)
        {
            auto it2 = freqDictionary.find(it.first);
            if(it2 == freqDictionary.end())
            {
                freqDictionary.emplace(it.first, it.second);
            }
            else
            {
                it2->second.push_back(it.second[0]);
            }
        }
    }
    freqDictionaryLock.unlock();
}

// подсчёт слов в документах и добавление результата в freqDictionary
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
        int currentDocIter = docsIter;
        docsIter++;
        docsLock.unlock();

        FreqDictionaryAppend(WordsEntry(currentDocIter));
    }
}

//Составление словаря
void InvertedIndex::CollectDictionary()
{
    freqDictionary.clear();
    docsIter = 0;
    size_t threadsNum = std::thread::hardware_concurrency();

    if(threadsNum > 1 && threadsNum > docs.size())
    {
        threadsNum = docs.size();
    }
    else
    {
        threadsNum = 1;
    }

    std::vector<std::thread> threads;
    threads.reserve(threadsNum);

    for(int i = 0; i < threadsNum; i++)
    {

        threads.emplace_back(std::thread([&]()
                                      {
                                          WordsCount();
                                      }));
    }

    for(int i = 0; i < threadsNum; i++)
    {
        if(threads[i].joinable()) threads[i].join();
    }
}

    // Обновить или заполнить базу документов, по которой будем совершать поиск
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> inputDocs)
{
    bool haveChanges = false;
    if(docs.size() != inputDocs.size())
    {
        docs.resize(inputDocs.size());
        haveChanges = true;
    }

    for(int i = 0; i < inputDocs.size(); i++)
    {
        if(docs[i] != inputDocs[i])
        {
            docs[i] = inputDocs[i];
            haveChanges = true;
        }
    }

    if(haveChanges)
    {
        CollectDictionary();
    }
}

    // Метод определяет количество вхождений слова word в загруженной базе документов
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const
{
    auto it = freqDictionary.find(word);
    if(it != freqDictionary.end())
    {
        return it->second;
    }
    else
    {
        std::vector<Entry> empty;
        return empty;
    }
}
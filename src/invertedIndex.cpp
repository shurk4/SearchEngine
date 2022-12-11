#include "invertedIndex.h"

// Конструктор для SearchServer
InvertedIndex::InvertedIndex(InvertedIndex &other)
{
    docs = other.docs;
    freq_dictionary = other.freq_dictionary;
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
            entry.doc_id = iter;
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
    freq_dictionaryLock.lock();

    if(freq_dictionary.empty())
    {
        freq_dictionary.insert(in.begin(), in.end());
    }
    else
    {
        for(auto it : in)
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

// подсчёт слов в документах и добавление результата в freq_dictionary
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
    freq_dictionary.clear();
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

    if(haveChanges)
    {
        CollectDictionary();
    }
}

    // Метод определяет количество вхождений слова word в загруженной базе документов
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const
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
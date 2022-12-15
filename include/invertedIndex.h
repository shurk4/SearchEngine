#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <thread>
#include <mutex>
#include <iostream>

struct Entry {
    size_t docId = 0, count = 0;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (docId == other.docId &&
                count == other.count);
    }
};


class InvertedIndex
{
    // список содержимого документов (коллекция для хранения текстов документов, в которой номер элемента в векторе определяет docId для формирования результата запроса;)
    std::vector<std::string> docs;

    // Частотный словарь (коллекция для хранения частоты слов, встречаемых в тексте. Entry представляет собой структуру:)
    std::map<std::string, std::vector<Entry>> freqDictionary;

    std::mutex docsLock;
    std::mutex freqDictionaryLock;
    int docsIter = 0;

    // подсчёт слов в документе
    void WordsCount();

    // составление словаря
    void CollectDictionary();

    // Подсчёт повторяющихся слов в документе
    std::map<std::string, std::vector<Entry>> WordsEntry(int iter);

    // Дополнение частотного словаря
    void FreqDictionaryAppend(std::map<std::string, std::vector<Entry>> in);

public:
    InvertedIndex() = default;

    // Конструктор для SearchServer
    InvertedIndex(InvertedIndex &other);

    // Обновить или заполнить базу документов, по которой будем совершать поиск
    void UpdateDocumentBase(std::vector<std::string> inputDocs);

    // Метод определяет количество вхождений слова word в загруженной базе документов
    std::vector<Entry> GetWordCount(const std::string& word) const;
};
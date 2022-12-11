#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <thread>
#include <mutex>
#include <iostream>

struct Entry {
    size_t doc_id = 0, count = 0;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};


class InvertedIndex
{
    // список содержимого документов (коллекция для хранения текстов документов, в которой номер элемента в векторе определяет doc_id для формирования результата запроса;)
    std::vector<std::string> docs;

    // Частотный словарь (коллекция для хранения частоты слов, встречаемых в тексте. Entry представляет собой структуру:)
    std::map<std::string, std::vector<Entry>> freq_dictionary;

    std::mutex docsLock;
    std::mutex freq_dictionaryLock;
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
    void UpdateDocumentBase(std::vector<std::string> input_docs);

    // Метод определяет количество вхождений слова word в загруженной базе документов
    std::vector<Entry> GetWordCount(const std::string& word) const;
};
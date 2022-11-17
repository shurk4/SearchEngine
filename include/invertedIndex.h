#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <thread>
#include <mutex>

struct Entry {
    size_t doc_id, count;
// Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};


class InvertedIndex
{
    std::vector<std::string> docs; // список содержимого документов (коллекция для хранения текстов документов, в которой номер элемента в векторе определяет doc_id для формирования результата запроса;)

    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь (коллекция для хранения частоты слов, встречаемых в тексте. Entry представляет собой структуру:)
    /*В коллекции freq_dictionary ключом служат слова из загруженных текстов, а значением
    — вектор из полей doc_id и count. Size_t — тип, используемый при задании размеров и
            индексации коллекций, обычно — это беззнаковый int, но правильнее писать именно
    size_t.
    ● doc_id — номер элемента в векторе docs, по которому хранится текст;
    count — число, которое обозначает, сколько раз ключевое слово встретилось в
            документе doc_id.*/

    std::mutex docsLock;
    std::mutex freq_dictionaryLock;
    std::mutex consoleLock;
    int docsIter = 0;

    // подсчёт слов в документе
    void WordsCount();

    // составление словаря
    void CollectDictionary();

public:
    InvertedIndex() = default;

    // Конструктор для SearchServer
    InvertedIndex(InvertedIndex &other);
/**
* Обновить или заполнить базу документов, по которой будем совершать
поиск*
@param texts_input содержимое документов
*/
    void UpdateDocumentBase(std::vector<std::string> const input_docs);
/**
 * * Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
    std::vector<Entry> GetWordCount(const std::string& word);

    void freq_dictionary_print();
};
#pragma once

#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include "invertedIndex.h"

struct RelativeIndex{
    size_t docId = 0;
    float rank = 0;
    bool operator == (const RelativeIndex& other) const {
        return (docId == other.docId && rank == other.rank);
    }
    bool operator < (const RelativeIndex& other) const
    {
        return (rank > other.rank || (rank == other.rank && docId < other.docId));
    }
};

class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе */
    explicit SearchServer(InvertedIndex& idx);

/**
* Метод обработки поисковых запросов
* @param queriesInput поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queriesInput);

    void setMaxResponse(int response);
private:
    InvertedIndex _index;
    int maxResponse = 5;
};
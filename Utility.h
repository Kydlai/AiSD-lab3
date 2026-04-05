#pragma once

#include <exception>
using namespace std;

enum statement
{
    EMPTY,
    RESERVED
};

class NoEmptySpaceException : public exception{
public:
    const char* what() const noexcept override {
        return "В сегменте нет свободного места для выделения памяти";
    }
};

class NullPtrException : public exception{
public:
    const char* what() const noexcept override {
        return "Обращение к nullptr";
    }
};

class NodeNotFoundException : public exception{
public:
    const char* what() const noexcept override {
        return "Не найден Node для удаление соответствующих данных";
    }
};
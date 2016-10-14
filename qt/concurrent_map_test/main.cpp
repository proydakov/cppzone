#include <iostream>

#include <QVector>
#include <QtConcurrent>
#include <QCoreApplication>

void filter(int& val)
{
    val += 10;
}

void redure(int val)
{
    std::cout << "val: " << val << std::endl;
}

void trace(const QVector<int>& data)
{
    std::for_each(data.begin(), data.end(), [](int val){
        std::cout << val << std::endl;
    });
}

int main(int argc, char *argv[])
{
    srand (time(NULL));

    size_t size = 2;1024 * 1024 * 1024;
    QVector<int> data;
    data.reserve(size);
    for(size_t i = 0; i < size; i++) {
        int value = rand() % 10000000;
        data.push_back(value);
    }

    std::cout << "map" << std::endl;

    trace(data);

    QtConcurrent::blockingMap(data, filter);

    std::cout << "result\n";
    trace(data);

    //std::cout << "press..." << std::endl;
    //std::getchar();

    return 0;
}

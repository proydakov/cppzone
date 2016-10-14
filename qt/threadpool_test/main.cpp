#include <iostream>

#include <QtConcurrent>
#include <QCoreApplication>

class Work : public QRunnable
{
public:
    Work(int num) :
        m_num(num)
    {
        std::cout << "work: " << m_num << std::endl;
    }

    ~Work() override
    {
        std::cout << "~work: " << m_num << std::endl;
    }

    void run() override
    {
        std::cout << "work run: " << m_num << std::endl;
        while(true);
    }

private:
    int m_num;
};

int main(int argc, char *argv[])
{
    QThreadPool pool;

    std::cout << "count: " << pool.activeThreadCount() << std::endl;
    std::cout << "count: " << pool.maxThreadCount() << std::endl;

    for(int i = 0; i < pool.maxThreadCount() + 2; i++) {
        QRunnable* functor = new Work(i);
        pool.start(functor);
    }

    std::cout << "count: " << pool.activeThreadCount() << std::endl;

    pool.waitForDone();
    return 0;
}

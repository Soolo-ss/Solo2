//
// Created by pc4 on 2018/7/11.
//

#ifndef SOLO_SINGLETON_H
#define SOLO_SINGLETON_H

template <typename T>
class Singleton
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton &&) = delete;

protected:
    Singleton() { }

    ~Singleton() { }
};


#endif //SOLO_SINGLETON_H

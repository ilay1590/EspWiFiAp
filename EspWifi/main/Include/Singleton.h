#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
    public:
      // deleting copy constructor
        Singleton(const Singleton& obj) = delete; 
        Singleton& operator=(Singleton const &) = delete;
        
        static T& Instance()
        {
            static T instance;
            return instance;
        }
    protected:
        Singleton() {}
        ~Singleton() {}
};

#endif // SINGLETON_H
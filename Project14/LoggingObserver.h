#pragma once

#include <string>
#include <list>
#include <fstream>
#include <iostream>
using std::string;
using std::list;
using std::ofstream;
using std::ostream;

class ILoggable {
protected:
    string logString;
public:
    ILoggable();
    ~ILoggable();
    ILoggable(const ILoggable&other);
    ILoggable& operator=(const ILoggable& other);
    virtual string stringToLog()const;
};
ostream& operator<<(ostream&os,const ILoggable& obj);

class Observer;

class Subject{
public:
public:
    virtual void attach(Observer* o);
    virtual void detach(Observer* o);
    virtual void notify(ILoggable* iLoggable);
    Subject();
    ~Subject();
    Subject(const Subject&other);
    Subject& operator=(const Subject& other);

    void copyObservers(const Subject& other);

    const list<Observer*>& getObservers()const;
private:
    list<Observer*> _observers;
};

ostream& operator<<(ostream& os, const Subject& obj);

class Observer {
public:
    Observer();
    ~Observer();
    Observer(const Observer&other);
    Observer& operator=(const Observer&other);
    virtual void update(ILoggable* iLoggable) = 0;
protected:
};

ostream& operator<<(ostream& os, const Observer& obj);

class LoggingObserver:public Observer
{
    ofstream ofs;
public:
    LoggingObserver(const string& filename);
    ~LoggingObserver();
    LoggingObserver(const LoggingObserver&other);
    LoggingObserver& operator=(const LoggingObserver& other);

    virtual void update(ILoggable* iLoggable);
};

ostream& operator<<(ostream& os, const LoggingObserver& obj);

void testLoggingObserver();
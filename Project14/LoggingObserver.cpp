#include "LoggingObserver.h"
using std::endl;

Observer::Observer() {
};

Observer::~Observer() {
}
Observer::Observer(const Observer& other)
{
    
}


Observer& Observer::operator=(const Observer& other) {
    return *this;
}

ostream& operator<<(ostream& os, const Observer& obj) {
    return os;
}

void Observer::update(ILoggable* iLoggable)
{
    
}

ILoggable& ILoggable::operator=(const ILoggable& other) {
    if (this != &other)
    {
        this->logString =other.logString;
    }
    return *this;
}

ostream& operator<<(ostream& os, const ILoggable& obj) {
    os <<obj.stringToLog();
    return os;
}

Subject::Subject() {
    ;
}
Subject::~Subject() {
    
}
Subject::Subject(const Subject& other)
    :_observers(other._observers)
{

}

Subject& Subject::operator=(const Subject& other) {
    if (this != &other) {
        this->_observers =other._observers;
    }
    return *this;
}

void Subject::copyObservers(const Subject& other)
{
    if (this != &other) {
        this->_observers =other._observers;
    }
}

const list<Observer*>& Subject::getObservers() const
{
    return this->_observers;
}


ostream& operator<<(ostream& os, const Subject& obj) {
    return os;
}


void Subject::attach(Observer* o) {
    if(o!=nullptr){
        _observers.push_back(o);
    }
};

void Subject::detach(Observer* o) {
    _observers.remove(o);
};
void Subject::notify(ILoggable* iLoggable) {
    list<Observer*>::iterator i = _observers.begin();
    for (; i != _observers.end(); ++i)
        (*i)->update(iLoggable);
};

LoggingObserver::LoggingObserver(const string& filename)
    :ofs(filename)
{
    
}

LoggingObserver::~LoggingObserver()
{
    
}

LoggingObserver::LoggingObserver(const LoggingObserver& other)
{
    
}

LoggingObserver& LoggingObserver::operator=(const LoggingObserver& other) {
    return *this;
}

void LoggingObserver::update(ILoggable* iLoggable)
{
    string str = iLoggable->stringToLog();
    ofs << str <<endl;
}

ostream& operator<<(ostream& os, const LoggingObserver& obj) {
    return os;
}

ILoggable::ILoggable()
{
}

ILoggable::~ILoggable()
{
    
}

ILoggable::ILoggable(const ILoggable& other)
    :logString(other.logString)
{
    
}



string ILoggable::stringToLog() const
{
    return logString;
}

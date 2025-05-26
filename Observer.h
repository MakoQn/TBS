#pragma once

#include "Defines.h"

class ISubject
{
public:
    virtual ~ISubject() {}

    virtual void attach(IObserver* observer) = 0;
    virtual void detach(IObserver* observer) = 0;
    virtual void notify(const GameEvent& event) = 0;
};

class IObserver
{
public:
    virtual ~IObserver() {}

    virtual void onNotify(const GameEvent& event) = 0;
};
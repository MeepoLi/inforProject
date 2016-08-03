#ifndef BASEGUI_H
#define BASEGUI_H

class BaseGui
{
public:
    virtual void run() = 0;
    virtual void connectSlots(){}
};

#endif // BASEGUI_H

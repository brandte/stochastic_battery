#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "power_update_m.h"
using namespace omnetpp;

class battery : public cSimpleModule{
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(battery);

void battery::initialize()
{
    // TODO - Generated method body
}

void battery::handleMessage(cMessage *msg)
{
    power_update *Power_pointer=check_and_cast<power_update *>(msg);
    int g = Power_pointer->getPower_consum();
    bubble("Battery hat was empfangen");
    EV <<"Power Level in der Batterie " << g;
}

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
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
    // TODO - Generated method body
}

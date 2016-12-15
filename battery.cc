#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "power_update_m.h"
using namespace omnetpp;

class battery : public cSimpleModule
{
    public:
        cMessage *battery_exhausted;
    private:

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(battery);

void battery::initialize()
{
    //code
}

void battery::handleMessage(cMessage *msg)
{
    power_update *Power_pointer=check_and_cast<power_update *>(msg);
    int g = Power_pointer->getPower_consum();
    bubble("Battery received!");
    EV<< "Power Level im "<< getParentModule()->getName() << " | Batterie" <<g;
}


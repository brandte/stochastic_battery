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
        cOutVector stat_capacity;
        cOutVector stat_power_level;
        int power_level;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(battery);

void battery::initialize()
{
    power_level=0;
    stat_capacity.setName("Battery Capacity");
    stat_power_level.setName("Power Consumption");
}

void battery::handleMessage(cMessage *msg)
{
    stat_power_level.record(power_level);
    power_update *Power_pointer=check_and_cast<power_update *>(msg);            //Auslesen des Power_upates
    power_level = Power_pointer->getPower_consum();
    stat_power_level.record(power_level);
    bubble("Battery received!");
    EV<< "Power Level im "<< getParentModule()->getName() << " | Batterie" <<power_level;
}


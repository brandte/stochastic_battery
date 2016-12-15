#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "power_update_m.h"
using namespace omnetpp;

class battery : public cSimpleModule{
    public:
        cMessage *imdead;
    private:
        cOutVector stat_capacity;
        cOutVector stat_power_level;
        int power_level;
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(battery);

void battery::initialize()
{
    stat_capacity.setName("Battery capacity");
    stat_power_level.setName("Power Level");
    power_level=0;
}

void battery::handleMessage(cMessage *msg){
    stat_power_level.record(power_level);
    power_update *Power_pointer=check_and_cast<power_update *>(msg);
    power_level = Power_pointer->getPower_consum();  //Auslesen des Power Levels
    stat_power_level.record(power_level);
    bubble("Battery hat was empfangen");
    EV <<"Power Level der " << getParentModule()->getName() << " Batterie " << power_level;
}

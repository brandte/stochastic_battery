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
        double power_level;
        long double capacity;                        //Wir nutzen double - Nachkommestellen max 15 !!! Das reicht
        double time;

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

    capacity = 2232; //[As] modified from Energizer CR2450 - 620 mAh
    stat_capacity.record(capacity);
    time=SIMTIME_DBL(simTime());

}

void battery::handleMessage(cMessage *msg)
{
    double usage = power_level*(SIMTIME_DBL(simTime())-time);
    capacity = capacity - usage;
    stat_capacity.record(capacity);

    stat_power_level.record(power_level);
    power_update *Power_pointer=check_and_cast<power_update *>(msg);            //Auslesen des Power_upates
    power_level = Power_pointer->getPower_consum();
    power_level = power_level/1000000;                                        //Umrechnung in Ampere
    stat_power_level.record(power_level);
    time=SIMTIME_DBL(simTime());

}


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
        double power_level;
        long double capacity;        //Wir nutzen double, das hat ~10^15 Genauigkeit; Verhältnis von Gesamtladung zu kleinste Entladung etwa 10^11. Wir schon passen
        double time;
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

    capacity=2232; //As aus Datenblatt 620mAh
    stat_capacity.record(capacity);
    time=simTime().dbl();
}

void battery::handleMessage(cMessage *msg){
    double usage=power_level*(simTime().dbl()-time);
    capacity=capacity-usage;
    stat_capacity.record(capacity);

    stat_power_level.record(power_level);
    power_update *Power_pointer=check_and_cast<power_update *>(msg);
    power_level = Power_pointer->getPower_consum();  //Auslesen des Power Levels
    power_level=power_level/1000000;        //Jetzt auch in Ampere
    stat_power_level.record(power_level);
    time=simTime().dbl();


}

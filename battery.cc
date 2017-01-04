#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <math.h>
#include "power_update_m.h"
using namespace omnetpp;

class battery : public cSimpleModule
{
    public:
        cMessage *battery_exhausted;
    private:
        cOutVector stat_capacity;
        cOutVector stat_power_level;
        cOutVector stat_recovery;
        double power_level;
        long double float_capacity;
        long double conversion;             //Conversion from SEU to As!! Richtige Reihenfolge beachten
        int64_t int_capacity;
        simtime_t time;
        char last_activity;
        bool dead;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(battery);

void battery::initialize()
{
    power_level=0;
    dead=0;
    last_activity='z';
    stat_capacity.setName("Battery Capacity");
    stat_power_level.setName("Power Consumption");
    stat_recovery.setName("Recovery of Energy");

    float_capacity = 108; //Unit: [As] modified from Energizer CR2450 - 620 mAh | 30mAh
    conversion=float_capacity/1e10;
    int_capacity = round(float_capacity/conversion);                  //measured in --> smallest energy unit "SEU"
    stat_capacity.record(float_capacity);
    time=simTime();
}

void battery::handleMessage(cMessage *msg)  // Berechnen des Verbrauchs aus der Vorperiode
{
if (dead==0){
    simtime_t delta_t = simTime()-time;
    int64_t usage = round(delta_t.dbl()*power_level/conversion);
    int_capacity -= usage;
    float_capacity = int_capacity*conversion;
    stat_capacity.record(float_capacity);

    if(int_capacity<=0){
        dead=1;
        battery_exhausted = new cMessage ("Battery exhausted");
        send(battery_exhausted, "port$o");
    }else{
    //Recovery condition
    if(last_activity!='z'){     //Initialisierung
        float q_0 = 0.95;
        float g_n = 3e-10;
        float g_c = 1e-4;
        float p=0;
        int64_t capacity_old = int_capacity;
        int64_t recovery;

        for (long int i=1; i <= usage; i++){
            p=q_0*exp(-int_capacity*g_n-i*g_c);
            if(0.01*intuniform(0,100)<p){
                int_capacity++;
                recovery = int_capacity-capacity_old;
            }
        }
        float_capacity = int_capacity*conversion;
        stat_capacity.record(float_capacity);
        stat_recovery.record(recovery);
    }
   }
    // Auslesen des Memory der eingegangenen Werte
    stat_power_level.record(power_level);
    power_update *Power_pointer=check_and_cast<power_update *>(msg);          //Auslesen des Power_upates (in MikroAmpere)
    power_level = Power_pointer->getPower_consum();
    power_level = power_level/1000000;                                        //Umrechnung in Ampere
    last_activity = Power_pointer->getCondition();
    stat_power_level.record(power_level);
    time=simTime();
}
}

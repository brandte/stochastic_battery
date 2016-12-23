#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <cmath>
#include "power_update_m.h"
using namespace omnetpp;

class battery : public cSimpleModule{
    public:
        cMessage *imdead;
    private:
        cOutVector stat_capacity;
        cOutVector stat_power_level;
        cOutVector stat_recovery;
        double power_level;
        long double float_capacity;  //Wir nutzen double, das hat ~10^15 Genauigkeit; Verhältnis von Gesamtladung zu kleinste Entladung etwa 10^11. Wir schon passen
        int64_t int_capacity;         //measured in kee kleinste Energie Einheiten SEU smalest Energy Units
        long double conversion;      //Conversion from SEU to As
        simtime_t time;
        char last_activity;
        bool dead;
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(battery);

void battery::initialize()
{
    stat_capacity.setName("Battery capacity");
    stat_power_level.setName("Power Level");
    stat_recovery.setName("How much is recovered");
    power_level=0;
    last_activity='z';
    dead=0;

    float_capacity=108;                    //As aus Datenblatt 620mAh(alt) jetzt 30mAh
    conversion=float_capacity/1e10;                   //Normierung auf SEU.
    int_capacity=round(float_capacity/conversion);
    stat_capacity.record(float_capacity);
    time=simTime();
}

void battery::handleMessage(cMessage *msg){
    if(dead==0){
        //Berechnen des Verbrauches in der vorangegangenen Periode.
        simtime_t delta_t=(simTime()-time);
        EV << getParentModule()->getName() << ": simtime_t delta_t: " << delta_t << "\n";
        int64_t usage=round(power_level/conversion*delta_t.dbl());
        EV << getParentModule()->getName() << ": int64_t usage: " << usage;
        int_capacity-=usage;
        float_capacity=int_capacity*conversion;
        stat_capacity.record(float_capacity);

        if (int_capacity<=0){
            dead=1;
            imdead=new cMessage("I am dead");
            send(imdead,"gate$o");
        }else{

            //Recovery
            if(last_activity!='z'){
                float q_0=0.95;
                float g_n=2e-10;
                float g_c=1e-5;
                float p=0;      //initialisation
                int64_t int_old_capacity =int_capacity;

                for(long int i=1; i<=usage; i++){
                    p=q_0*exp(-int_capacity*g_n-i*g_c);
                    if(0.01*intuniform(0,100)<p){
                        int_capacity++;
                    }
                }
                float_capacity=int_capacity*conversion;
                stat_capacity.record(float_capacity);
                stat_recovery.record(int_capacity-int_old_capacity);
            }



            //Auslesen und Speicher der Eingangenen Werte.
            stat_power_level.record(power_level);
            power_update *Power_pointer=check_and_cast<power_update *>(msg);
            power_level = Power_pointer->getPower_consum();  //Auslesen des Power Levels (in Mikroampere)
            power_level=power_level/1000000;        //Jetzt auch in Ampere
            last_activity = Power_pointer->getCurrent_activity();
            stat_power_level.record(power_level);
            time=simTime();
        }
    }
}

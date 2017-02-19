#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <cmath>
using namespace omnetpp;

#include "power_update_m.h"         //Sends the power level and state from transceiver to battery.

class battery : public cSimpleModule{
    public:
        cMessage *imdead;           //This message tells the transceiver when the battery is empty
    private:
        //The cOutVectors are for data collection
        cOutVector stat_capacity;   //How much capacity is in the battery
        cOutVector stat_recovery;   //How much energy is recovered in this step
        //cOutVector stat_power_level;

        double power_level;          //How much power is drained by the transceiver
        long double float_capacity;  //The capacity as a floating point in [As]. It needs to be very big and very precise. Will mainly be used for input, as well as analysis output.
        int64_t int_capacity;        //This one is the capacity in [SEU] (called N in the formula). All calculations will be done with this discrete one.
        long double conversion;      //Conversion from SEU to As
        simtime_t last_time;         //This variable will store the time, when the power consumption was changed last time.
        char last_activity;          //This variable will store the activity that was performed by the transceiver at the last power level change.
        bool dead;                   //When the battery is empty we change this boolean and send the information to the transceiver.

        double q_0;     //Important variable for the stochastic formula. See the documentation for details. Inherited from omnetpp.ini
        double g_n;     //Important variable for the stochastic formula. See the documentation for details. Inherited from omnetpp.ini
        double g_c;     //Important variable for the stochastic formula. See the documentation for details. Inherited from omnetpp.ini
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(battery);

void battery::initialize()
{
    q_0=par("q_0");    //Inheritance from the omnetpp.ini
    g_n=par("g_N");    //Inheritance from the omnetpp.ini
    g_c=par("g_C");    //Inheritance from the omnetpp.ini

    stat_capacity.setName("Battery capacity");      //For analysis
    //stat_power_level.setName("Power Level");      //For analysis
    stat_recovery.setName("How much is recovered"); //For analysis

    power_level=0;
    last_activity='z';
    dead=0;

    double N=par("N");
    float_capacity=par("battery_capacity");        //From onetpp.ini
    conversion=float_capacity/N;                   //Normalization of the input to SEU.
    int_capacity=round(float_capacity/conversion);
    stat_capacity.record(float_capacity);
    last_time=simTime();
}

void battery::handleMessage(cMessage *msg){
    if(dead==0){    //When we are dead (empty) we will do nothing

        //calculation of the power consumption in the previous time interval.
        simtime_t delta_t=(simTime()-last_time);
        int64_t usage=round(power_level/conversion*delta_t.dbl());  //how much energy was consumed
        int_capacity-=usage;                                        //update capacity
        float_capacity=int_capacity*conversion;         //For external analysis
        stat_capacity.record(float_capacity);           //The graph looks better when we include some redundancy.

        //Recovery
        if (int_capacity<=0){
            //When the last consumption drained to much energy, we will not restore anything.
            dead=1;
            imdead=new cMessage("I am dead");   //Tell the transceiver that we are empty.
            send(imdead,"gate$o");
            EV << getParentModule()->getName() << " is empty.";
        }else{
            if(last_activity!='z'){         //We can not recover energy from the sleeping time, because it is considered as a constant background drain.
                float p=0;                  //Initialization
                int64_t int_old_capacity =int_capacity;
                for(long int i=1; i<=usage; i++){
                    //This is the part of the code that eats a lot of time. This loop is called very often and has to execute an exponential function every time.
                    //This is the "core element" of the stochastic battery model
                    p=q_0*exp(-int_capacity*g_n-i*g_c);   //We calculate a probability that a SEU is recovered and we go up a step in the markov chain.
                    if(0.001*intuniform(0,1000)<p){       //Then we draw a random number an check if the propability is hit.
                        int_capacity++;
                    }
                }
                float_capacity=int_capacity*conversion;     //For external analysis.
                stat_capacity.record(float_capacity);
                stat_recovery.record(int_capacity-int_old_capacity);
            }

            //read and store the incoming values, so we can use them in the next turn.
            //stat_power_level.record(power_level);                               //The graph looks better when we include some redundancy.
            power_update *Power_pointer=check_and_cast<power_update *>(msg);    //call the message
            power_level = Power_pointer->getPower_consum();                     //read the submitted power level. In [Microampere]
            power_level=power_level/1000000;                                    //Convert to [Ampere]
            last_activity = Power_pointer->getCurrent_activity();               //Store the activity
            //stat_power_level.record(power_level);
            last_time=simTime();                                                //We need this to calculate the delta later.
        }
    }
}

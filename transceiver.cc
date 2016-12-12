#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "power_update_m.h"
using namespace omnetpp;

class transceiver : public cSimpleModule
{
  private:
    //float power_lvl;    //The power level of the transceiver that will later be send to the battery.
    int power_consumption;           //Power level of the Batter in micro-Ampere
    cMessage *measuring_interval;    //This one is needed for the sender, to set the measure and send interval.
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void power_level(std::string activity);
};

Define_Module(transceiver);

void transceiver::initialize(){
    if (strcmp("Sender", getParentModule()->getName()) == 0) {
        send (new cMessage(),"transmission$o");
        power_level("send");
        measuring_interval = new cMessage("measuring interval");
        scheduleAt(normal(1800,60),measuring_interval);
    }
}

void transceiver::handleMessage(cMessage *msg)
{
    cGate *arrivalGate = msg->getArrivalGate();
    if (arrivalGate==NULL){ //first self-message --> Node is Sender
        send (new cMessage(),"transmission$o");
        power_level("send");
        scheduleAt(simTime()+normal(1800,60),measuring_interval);
    }else{
        //code
    }
}

void transceiver::power_level(std::string activity)
{
    if (strcmp(par("transceiver_type"),("CC2530"))==0){
        if (activity=="send"){
            power_consumption=33500;
            bubble("Sending");

        }else if(activity=="receive"){
            power_consumption=24300;
            bubble("Receiving");
        }else{      //if state("sleeping"/"idle")
            power_consumption=200;
            bubble("sleeping/idle");
        }

    }else if (strcmp(par("transceiver_type"),("ESP8266"))==0){
        if (activity=="send"){
            power_consumption=215000;
            bubble("Sending");

        }else if(activity=="receive"){
            power_consumption=60000;
            bubble("Receiving");
        }else{      //if state("sleeping"/"idle")
            power_consumption=10;
            bubble("sleeping/idle");
        }

    }else if(strcmp(par("transceiver_type"),("CC2650"))==0){
        if (activity=="send"){
                  power_consumption=9100;
              bubble("Sending");

          }else if(activity=="receive"){
              power_consumption=6100;
              bubble("Receiving");
          }else{      //if state("sleeping"/"idle")
              power_consumption=1;
              bubble("sleeping/idle");
          }

    }else if(strcmp(par("transceiver_type"),("RFD22301"))==0){
        if (activity=="send"){
               power_consumption=12000;
               bubble("Sending");

           }else if(activity=="receive"){
               power_consumption=12000;
               bubble("Receiving");
           }else{      //if state("sleeping"/"idle")
               power_consumption=4;
               bubble("sleeping/idle");
           }
    }
    power_update *pwrupd = new power_update();
    pwrupd->setPower_consum(power_consumption);
    send(pwrupd,"battery_connection$o");
}


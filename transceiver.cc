#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

class transceiver : public cSimpleModule{
  private:
    int power_consumption;      //The power level of the transceiver that will be transmitted to the battery. In Microampere
    cMessage *measuring_interval;    //This one will be a self message, to set the measure and send interval.
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void power_level(std::string activity);// override;
};

Define_Module(transceiver);

void transceiver::initialize(){
    if (strcmp("Sender", getName()) == 0) {
        send (new cMessage(),"gate$o");
        power_level("send");
        measuring_interval = new cMessage("measuring interval");
        scheduleAt(normal(1800,60),measuring_interval);
    }
}

void transceiver::handleMessage(cMessage *msg)
{
    cGate *arrivalGate = msg->getArrivalGate();
    if (arrivalGate==NULL){ //dann war das eine self-message und wir sind der Messpunkt
        send (new cMessage(),"gate$o");
        power_level("send");
        scheduleAt(simTime()+normal(1800,60),measuring_interval);
    }else{      //dann sind wir der Empfänger und haben eine Nachricht von Außen bekommen
        //hier haben wir jetzt eine Nachricht empfangen und müssen unseren Pegel dementsprechend anpassen.
        //später...
        power_level("receive");
    }
}

void transceiver::power_level(std::string activity){

    if (strcmp(par("transceiver_type"),("CC2530"))==0){
        if (activity=="send"){
            power_consumption=33500;
        }else if(activity=="receive"){
            power_consumption=24300;
        }else{      //Sleeping/idle
            power_consumption=200;
        }

    }else if (strcmp(par("transceiver_type"),("ESP8266"))==0){
        if (activity=="send"){
            power_consumption=215000;
        }else if(activity=="receive"){
            power_consumption=60000;
        }else{      //Sleeping/idle
            power_consumption=10;
        }
    }else if(strcmp(par("transceiver_type"),("CC2650"))==0){
        if (activity=="send"){
            power_consumption=9100;
        }else if(activity=="receive"){
            power_consumption=6100;
        }else{      //Sleeping/idle
            power_consumption=1;
        }

    }else if(strcmp(par("transceiver_type"),("RFD22301"))==0){
        if (activity=="send"){
            power_consumption=12000;
        }else if(activity=="receive"){
            power_consumption=12000;
        }else{      //Sleeping/idle
            power_consumption=4;
        }
    }
    //EV<<"Power level " << power_consumption;
}

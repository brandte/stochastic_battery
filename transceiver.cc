#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

#include "power_update_m.h"     //Sends the power level from transceiver to battery


class transceiver : public cSimpleModule{
  public:
    //cMessage *data_load;                //Data that is send between sender and receiver.

  private:
    int power_consumption;      //The power level of the transceiver that will be transmitted to the battery. In Microampere
    cMessage *measuring_interval_SM;    //This one will be a self message, to set the measure and send interval.
    cMessage *sending_time_SM;          //How long does the sending take? This SM will regulate.

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void power_level(std::string activity);// override;
};

Define_Module(transceiver);

void transceiver::initialize(){
    power_level("sleep");
    if (strcmp("Sender", getParentModule()->getName()) == 0) {
        cMessage *l_data_load = new cMessage("data_load");
        send (l_data_load,"transmission$o");
        power_level("send");
        int sending_duration=3;
        sending_time_SM = new cMessage("sending time SM");
        scheduleAt(sending_duration,sending_time_SM);
        measuring_interval_SM = new cMessage("measuring interval");
        scheduleAt(normal(1800,60),measuring_interval_SM);
    }
}

void transceiver::handleMessage(cMessage *msg){
    if (msg==measuring_interval_SM){ //dann war das eine self-message und wir sind der Messpunkt
        cMessage *l_data_load = new cMessage("data_load");
        send (l_data_load,"transmission$o");
        power_level("send");
        int sending_duration=3;
        scheduleAt(simTime()+sending_duration,sending_time_SM);
        scheduleAt(simTime()+normal(1800,60),measuring_interval_SM);
    }else if(msg==sending_time_SM){
        power_level("sleep");
    }else if(strcmp(msg->getName(),"data_load")==0){      //dann sind wir der Empf‰nger und haben eine Nachricht von Auﬂen bekommen
        EV << getParentModule()->getName() << " hat eine Nachricht empfangen.";
        power_level("receive");
        delete msg;
    }else{
        EV << "Irgendwas laeuft hier falsch: " << getParentModule()->getName() << "\n";
        EV << msg->getName();
        delete msg;
    }
}

void transceiver::power_level(std::string activity){

    if (strcmp(par("transceiver_type"),("CC2530"))==0){
        if (activity=="send"){
            power_consumption=33500;
        }else if(activity=="receive"){
            power_consumption=24300;
        }else{      //Sleeping/idle; default case
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
        }else{      // Sleeping/idle
            power_consumption=4;
        }
    }
    power_update *pwr_upd=new power_update();
    pwr_upd->setPower_consum(power_consumption);
    send(pwr_upd,"battery_connection$o");
}

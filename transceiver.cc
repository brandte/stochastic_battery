#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include "power_update_m.h" //msg: sends power update from transceiver to battery
//msg: controls duration of sending time and, hence, gives information about the power used for activity

using namespace omnetpp;

class transceiver : public cSimpleModule
{
    private:
        //float power_lvl;                      //The power level of the transceiver that will later be send to the battery.
        int power_consumption;                  //Power level of the Batter in micro-Ampere
        cMessage *measuring_interval_SM;        //This one is needed for the sender, to set the measure and send interval.
        cMessage *sending_time_SM;              // Sending time msg, refers to Data_laod and transmission speed.
        //cMessage *data_load;                    //data from sender to receiver. Also describes as payload.

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void power_level(std::string activity);
};

Define_Module(transceiver);

void transceiver::initialize(){
    power_level("sleep");
    if (strcmp("Sender", getParentModule()->getName()) == 0) {
        cMessage *l_data_load = new cMessage ("data_load");
        send(l_data_load, "transmission$o");
        sending_time_SM = new cMessage ("sending time");
        measuring_interval_SM = new cMessage("measuring interval");
        power_level("send");
        int sending_duration = 3;                                       //Equation with frequency and transmitting speed.
        scheduleAt(simTime()+sending_duration, sending_time_SM);
        scheduleAt(simTime()+normal(1800,60),measuring_interval_SM);
    }
}

void transceiver::handleMessage(cMessage *msg)
{
    if (msg==measuring_interval_SM){                                    //first self-message --> Node is Sender
        cMessage *l_data_load = new cMessage ("data_load");
        send(l_data_load, "transmission$o");
        power_level("send");
        int sending_duration = 3;                                       //Berechnung muss noch erfolgen!!!!!!!!!!!
        scheduleAt(simTime()+sending_duration, sending_time_SM);
        scheduleAt(simTime()+normal(1800,60),measuring_interval_SM);
    }else if (strcmp(msg->getName(),"data_load")==0){
        power_level("receive");
        delete msg;
    }else if(msg==sending_time_SM){
        power_level("sleep");
    }else{
        EV<< "ERROR+++ERROR+++ERROR+++ERROR+++ERROR+++ERROR+++ERROR+++ERROR+++";
        EV<< msg->getName();
        delete msg;
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
        }else{            //if state("sleeping"/"idle")
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

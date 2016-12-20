#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include "power_update_m.h" //msg: sends power update from transceiver to battery
//msg: controls duration of sending time and, hence, gives information about the power used for activity

using namespace omnetpp;

class transceiver : public cSimpleModule
{
    public:
        cMessage *battery_exhausted;

    private:
        //float power_lvl;                      //The power level of the transceiver that will later be send to the battery.
        int power_consumption;                  //Power level of the Batter in micro-Ampere
        cMessage *measuring_interval_SM;        //This one is needed for the sender, to set the measure and send interval.
        cMessage *sending_time_SM;              // Sending time msg, refers to Data_laod and transmission speed.
                                                //data from sender to receiver. Also describes as payload.
        double sending_duration;
        cMessage *wake_up_SM;
        bool dead;

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void power_level(std::string activity);
        virtual double sending_duration_function();
};

Define_Module(transceiver);

void transceiver::initialize(){
    dead=0;
    power_level("sleep");
    sending_duration =sending_duration_function();                                       //Berechnung muss noch erfolgen!!!!!!!!!!!
    int l_sending_interval = par("sending_interval");

    if(l_sending_interval!=0){                                                           // I'm a Sender
        if (strcmp("Sender", getParentModule()->getName())==0) {
            //cMessage *l_data_load = new cMessage ("data_load");
            //send(l_data_load, "transmission$o");
            sending_time_SM = new cMessage ("sending time");
            measuring_interval_SM = new cMessage("measuring interval");
            //power_level("send");
            //scheduleAt(simTime()+sending_duration, sending_time_SM);
            scheduleAt(simTime()+l_sending_interval,measuring_interval_SM);
        }else{                                                                          // I'm a Receiver
            wake_up_SM = new cMessage ("wake_up_SM_Re");
            scheduleAt(0.95*l_sending_interval,wake_up_SM);
        }
    }

}                           // RECEIVER wird an dieser Stelle nicht initalisiert, weswegen die ersten Nachricht keinen Strom verbraucht
                            // Unschärfe wird an dieser Stelle akzeptiert

void transceiver::handleMessage(cMessage *msg)
{
    if(dead==0){

        if (msg==measuring_interval_SM){                                    //first self-message --> Node is Sender
            cMessage *l_data_load = new cMessage ("data_load");
            send(l_data_load, "transmission$o");
            power_level("send");
            scheduleAt(simTime()+sending_duration, sending_time_SM);
            scheduleAt(simTime()+par("sending_interval"),measuring_interval_SM);
        }else if (strcmp(msg->getName(),"data_load")==0){
            power_level("sleep");
            double l_sending_interval = par("sending_interval");
            scheduleAt(simTime()+0.95*l_sending_interval, wake_up_SM);
            delete msg;
        }else if(msg==sending_time_SM){
            power_level("sleep");
        }else if(msg==wake_up_SM){
            power_level("receive");                                         //assumption of a perfect channel and no information lost
        }else if(msg==battery_exhausted){
            dead=1;
            delete msg;
            EV<< "Battery from" << getParentModule()->getName() << "is empty";
        }else{
            EV<< "ERROR+++ERROR+++ERROR+++ERROR+++ERROR+++ERROR+++ERROR+++ERROR";
            EV<< msg->getName();
            delete msg;
        }

    }
}

double transceiver::sending_duration_function()
{
    double l_sending_duration=0;
    int l_message_length = par("message_length");
    l_message_length=64*ceil(l_message_length/64);               //extended message length -> modulo 64

    if (strcmp(par("transceiver_type"),("CC2530"))==0){             //assuming BPSK
        l_sending_duration=(l_message_length/250000);
    }else if(strcmp(par("transceiver_type"),("ESP8266"))==0){
        l_sending_duration=(l_message_length/54000000);
    }else if(strcmp(par("transceiver_type"),("CC2650"))==0){
        l_sending_duration=(l_message_length/1000000);
    }else/*(strcmp(par("transceiver_type"),("RFD22301"))==0)*/{
        l_sending_duration=(l_message_length/250000);
    }
    return l_sending_duration;
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
        }else{            //if state("sleeping")
            power_consumption=200;
            bubble("sleeping");
        }

    }else if (strcmp(par("transceiver_type"),("ESP8266"))==0){
        if (activity=="send"){
            power_consumption=215000;
            bubble("Sending");
        }else if(activity=="receive"){
            power_consumption=60000;
            bubble("Receiving");
        }else{      //if state("sleeping")
            power_consumption=10;
            bubble("sleeping");
        }

    }else if(strcmp(par("transceiver_type"),("CC2650"))==0){
        if (activity=="send"){
                  power_consumption=9100;
              bubble("Sending");
        }else if(activity=="receive"){
              power_consumption=6100;
              bubble("Receiving");
        }else{      //if state("sleeping")
              power_consumption=1;
              bubble("sleeping");
        }

    }else if(strcmp(par("transceiver_type"),("RFD22301"))==0){
        if (activity=="send"){
               power_consumption=12000;
               bubble("Sending");
        }else if(activity=="receive"){
               power_consumption=12000;
               bubble("Receiving");
        }else{      //if state("sleeping")
               power_consumption=4;
               bubble("sleeping");
        }
    }

    char short_activity='e';            //using this for implementing variable in power_update.msg
    if (activity=="send"){              // e == ERROR
        short_activity='s';
    } else if(activity=="receive"){
        short_activity='r';
    }else if (activity=="sleep"){
        short_activity='z';             // i==leep
    }

    power_update *pwrupd = new power_update();
    pwrupd->setCondition(short_activity);
    pwrupd->setPower_consum(power_consumption);
    send(pwrupd,"battery_connection$o");
}

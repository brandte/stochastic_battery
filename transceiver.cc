#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

#include "power_update_m.h"     //Sends the power level from transceiver to battery


class transceiver : public cSimpleModule{
  public:
    cMessage *imdead;
  private:
    int power_consumption;      //The power level of the transceiver that will be transmitted to the battery. In Microampere
    bool dead;
    cMessage *measuring_interval_SM;    //This one will be a self message, to set the measure and send interval.
    cMessage *sending_time_SM;          //How long does the sending take? This SM will regulate.
    cMessage *wakeup_SM;                //The SM that will wake up the receiver.
    double sending_duration;


  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void power_level(std::string activity);// override;
    virtual double sending_duration_func();
};

Define_Module(transceiver);

void transceiver::initialize(){
    dead=0;
    power_level("sleep");
    int l_sending_interval=par("sending_interval");

    if (l_sending_interval!=0){
        if (strcmp("Sender", getParentModule()->getName()) == 0){
            sending_time_SM = new cMessage("sending time SM");
            measuring_interval_SM = new cMessage("measuring interval");
            scheduleAt(par("sending_interval"),measuring_interval_SM);
        }else{  //ich bin der Receiver
            wakeup_SM=new cMessage("wake up SM for Receiver");
            scheduleAt(0.95*l_sending_interval,wakeup_SM);
        }
    }

}

void transceiver::handleMessage(cMessage *msg){
    if(dead==0){
        if (msg==measuring_interval_SM){ //dann war das eine self-message und wir sind der Messpunkt
            cMessage *l_data_load = new cMessage("data_load");
            send (l_data_load,"transmission$o");
            power_level("send");
            scheduleAt(simTime()+sending_duration,sending_time_SM);
            scheduleAt(simTime()+par("sending_interval"),measuring_interval_SM);
        }else if(msg==sending_time_SM){
            power_level("sleep");
        }else if(strcmp(msg->getName(),"data_load")==0){      //dann sind wir der Empf‰nger und haben eine Nachricht von Auﬂen bekommen
            EV << getParentModule()->getName() << " hat eine Nachricht empfangen.";
            power_level("sleep");
            double l_sending_interval=par("sending_interval");
            scheduleAt(simTime()+0.95*l_sending_interval,wakeup_SM);
            delete msg;
        }else if(msg==wakeup_SM){
            power_level("receive");         //Wir nehmen einen perfekten Kanal an. Keine Nachricht geht verloren, daher keine weiteren Fallunterscheidungen.
        }else if(msg==imdead){              //Battery is empty.
            dead=1;
            EV << getParentModule()->getName() << "s battery is empty.";
            delete msg;
        }else{
            EV << "Irgendwas laeuft hier falsch: " << getParentModule()->getName() << "\n";
            EV << msg->getName();
            delete msg;
        }
    }
}

double transceiver::sending_duration_func(){
    double l_sending_duration;
    int l_message_length=par("message_length");
    l_message_length=ceil(l_message_length/64)*64;

    if (strcmp(par("transceiver_type"),("CC2530"))==0){
        l_sending_duration=l_message_length/250000;             //Assuming BPSK
    }else if (strcmp(par("transceiver_type"),("ESP8266"))==0){
        l_sending_duration=l_message_length/54000000;
    }else if(strcmp(par("transceiver_type"),("CC2650"))==0){
        l_sending_duration=l_message_length/1000000;
    }else{                                                      //(strcmp(par("transceiver_type"),("RFD22301"))==0){
        l_sending_duration=l_message_length/250000;
    }
    return l_sending_duration;
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

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <cmath>
using namespace omnetpp;

#include "power_update_m.h"     //Sends the power level and state from transceiver to battery.

class transceiver : public cSimpleModule{
  public:
    cMessage *imdead;            //This cMessage comes from the battery, when it is empty.
  private:
    int power_consumption;       //The power level of the transceiver that will be transmitted to the battery. In microampere
    bool dead;                   //When the power is empty this one will be set to zero.
    cMessage *measuring_interval_SM;    //This one will be a self message (SM), to set the "measure & send"-interval.
    cMessage *sending_time_SM;          //How long does the sending take? This SM will regulate.
    cMessage *wakeup_SM;                //The SM that will wake up the receiver.
    double sending_duration;            //How long will the sending take?  Based on data rate and message length.
    double receiver_offtime;            //The receiver has a receiving window. With this value we simulate that.
    int transceiver_datarate;           //Data rate. Inherited from omnetpp.ini

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void power_level(std::string activity);        //This one will prepare and send the power_update message.
    virtual double sending_duration_func();                //This one gives us the sending duration
};

Define_Module(transceiver);

void transceiver::initialize(){
    dead=0;                     //We are alive
    power_level("sleep");       //Everyone starts sleeping
    int l_sending_interval=par("sending_interval");
    sending_duration=sending_duration_func();

    if (l_sending_interval!=0){         //When this is zero we do nothing.
        if (strcmp("Sender", getParentModule()->getName()) == 0){
            transceiver_datarate=par("transceiver_datarate");
            sending_time_SM = new cMessage("sending time SM");              //We initialize the SM
            measuring_interval_SM = new cMessage("measuring interval");     //same here.
            scheduleAt(par("sending_interval"),measuring_interval_SM);      //The sender interval is started. When this SM will arrive the first message will be send.
        }else{  //receiver
            receiver_offtime=par("receiver_offtime");
            wakeup_SM=new cMessage("wake up SM for Receiver");              //SM is initialized
            scheduleAt(receiver_offtime*l_sending_interval,wakeup_SM);      //and send. The receiver will be active for a while, before it receives the message from the sender.
        }
    }
}

void transceiver::handleMessage(cMessage *msg){
    if(dead==0){
        //when dead==0 the battery is empty an we do nothing at all.
        if (msg==measuring_interval_SM){
            //when this SM hits, we are the sender and it is time to send a message to the receiver.
            cMessage *l_data_load = new cMessage("data_load");      //This is the message that is send to the receiver. It is empty, because in this scenario we don't care.
            send (l_data_load,"transmission$o");
            power_level("send");                                    //Call of the function, to interact with the battery.
            scheduleAt(simTime()+sending_duration,sending_time_SM);                 //This self message determines when the status will be set back to sleep
            scheduleAt(simTime()+par("sending_interval"),measuring_interval_SM);    //This SM will activate the next sending process.
        }else if(msg==sending_time_SM){
            //when the sending time is over, we tell the battery that we go back to sleep power level.
            power_level("sleep");
        }else if(strcmp(msg->getName(),"data_load")==0){
            //When this comes in, we are the receiver and received a message from outside.
            //EV << getParentModule()->getName() << " has received a message.";
            power_level("sleep");               //We do not consider the time that it takes to receive and process the message. It should be much smaller than the receiving window so it can be neglected.
            double l_sending_interval=par("sending_interval");
            scheduleAt(simTime()+receiver_offtime*l_sending_interval,wakeup_SM);    //This SM wakes us, when the receiving window should open again.
            delete msg;
        }else if(msg==wakeup_SM){
            //The receiving window opens again. We assume a perfect channel so that a message will arrive in any case to close the window.
            power_level("receive");
        }else if(msg==imdead||strcmp(msg->getName(),"I am dead")==0){
            //When the battery is empty it sends this message.
            dead=1; //In the next call of handleMessage nothing will happen. As in every future call.
            EV << getParentModule()->getName() << "s battery is empty.";
            delete msg;
        }else{      //This case should not be occure.
            EV << "Something is wrong here: " << getParentModule()->getName() << "\n";
            EV << msg->getName();
            delete msg;
        }
    }
}

double transceiver::sending_duration_func(){
    //This function calculates how long the sender will be sending depending on the given parameters from the omnetpp.ini
    double l_sending_duration;
    double l_message_length=par("message_length");   //in Byte
    l_message_length=ceil(l_message_length/8)*64;    //in Bit

    if (strcmp(par("transceiver_type"),("CC2530"))==0){
        l_sending_duration=l_message_length/250000;             //Assuming BPSK
    }else if (strcmp(par("transceiver_type"),("ESP8266"))==0){
        l_sending_duration=l_message_length/54000000;
    }else if(strcmp(par("transceiver_type"),("CC2650"))==0){
        l_sending_duration=l_message_length/1000000;
    }else if(strcmp(par("transceiver_type"),("RFD22301"))==0){
        l_sending_duration=l_message_length/250000;
    }else{
        l_sending_duration=l_message_length/(transceiver_datarate*1000);
    }

    return l_sending_duration;
}

void transceiver::power_level(std::string activity){
    //When you give this function an activity ("send", "receive", "sleep") it will tell the battery how much power is needed for that.
    //It is called when the activity starts and does not say when it ends. It will just send the next state.
    //The consumption rates are based on the input from the omnetpp.ini

    if (strcmp(par("transceiver_type"),("CC2530"))==0){
        if (activity=="send"){
            power_consumption=33500;
        }else if(activity=="receive"){
            power_consumption=24300;
        }else{      //Sleeping; default case
            power_consumption=200;
        }

    }else if (strcmp(par("transceiver_type"),("ESP8266"))==0){
        if (activity=="send"){
            power_consumption=215000;
        }else if(activity=="receive"){
            power_consumption=60000;
        }else{      //Sleeping
            power_consumption=10;
        }
    }else if(strcmp(par("transceiver_type"),("CC2650"))==0){
        if (activity=="send"){
            power_consumption=9100;
        }else if(activity=="receive"){
            power_consumption=6100;
        }else{      //Sleeping
            power_consumption=1;
        }

    }else if(strcmp(par("transceiver_type"),("RFD22301"))==0){
        if (activity=="send"){
            power_consumption=12000;
        }else if(activity=="receive"){
            power_consumption=12000;
        }else{      // Sleeping
            power_consumption=4;
        }
    }else{      //nothing is given, we use the manually set values.
        if (activity=="send"){
            power_consumption=par("transceiver_energy_sending");
        }else if(activity=="receive"){
            power_consumption=par("transceiver_energy_receiving");
        }else{      // Sleeping
            power_consumption=par("transceiver_energy_sleeping");
        }
    }
    //The message does not allow strings so we have to set a char.
    char short_activity='e';        //Initialized with e for error, because it should in any case be changed.
    if (activity=="send"){short_activity='s';}
    else if(activity=="receive"){short_activity='r';}
    else if(activity=="sleep"){short_activity='z';}

    power_update *pwr_upd=new power_update();
    pwr_upd->setPower_consum(power_consumption);
    pwr_upd->setCurrent_activity(short_activity);
    send(pwr_upd,"battery_connection$o");       //And here the message goes...
}

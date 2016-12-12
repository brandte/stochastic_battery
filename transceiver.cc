#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

class transceiver : public cSimpleModule
{
  private:
    //float power_lvl;    //The power level of the transceiver that will later be send to the battery.
    cMessage *measuring_interval;    //This one is needed for the sender, to set the measure and send interval.
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(transceiver);

void transceiver::initialize()
{
    if (strcmp("Sender", getName()) == 0) {
        send (new cMessage(),"gate$o");
        measuring_interval = new cMessage("measuring interval");
        scheduleAt(normal(1800,60),measuring_interval);
    }
}

void transceiver::handleMessage(cMessage *msg)
{
    cGate *arrivalGate = msg->getArrivalGate();
    if (arrivalGate==NULL){ //dann war das eine self-message und wir sind der Messpunkt
        send (new cMessage(),"gate$o");
        scheduleAt(simTime()+normal(1800,60),measuring_interval);
    }else{      //dann sind wir der Empfänger und haben eine Nachricht von Außen bekommen
        //hier haben wir jetzt eine Nachricht empfangen und müssen unseren Pegel dementsprechend anpassen.
        //später...
        bubble("Hurray");
    }
}

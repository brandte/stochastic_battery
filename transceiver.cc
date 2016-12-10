#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

class transceiver : public cSimpleModule
{
  private:

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(transceiver);

void transceiver::initialize()
{
    // TODO - Generated method body
}

void transceiver::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

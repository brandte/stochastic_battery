package stochastic_battery;

import ned.IdealChannel;


module Node
{
    gates:
        inout port;

    submodules:
        transceiver:transceiver;
        battery:battery;
        
    connections:
        transceiver.transmission <--> port;
        transceiver.battery_connection <--> battery.port;
}

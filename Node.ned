package stochastic_battery;

//Compound module that combines the transceiver and the battery to a node.

module Node
{
    gates:
        inout port;
    submodules:
        transceiver:transceiver; 
        battery:battery;  
    connections:
        transceiver.battery_connection <--> battery.gate;
        transceiver.transmission <--> port;   
}

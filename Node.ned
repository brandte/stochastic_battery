package stochastic_battery;

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

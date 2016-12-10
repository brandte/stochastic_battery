package stochastic_battery;

network Netzwerk{
   submodules:
        Sender: transceiver;
        Receiver: transceiver;
        
    connections:
        Sender.gate <--> {  delay = 100ms; } <--> Receiver.gate;
}  
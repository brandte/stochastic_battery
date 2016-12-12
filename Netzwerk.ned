package stochastic_battery;

network Netzwerk{		//Nice and simple
   submodules:
        Sender: transceiver;
        Receiver: transceiver;
        
    connections:
        Sender.gate <--> {  delay = 100ms; } <--> Receiver.gate;
}  
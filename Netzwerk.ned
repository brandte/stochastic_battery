package stochastic_battery;

network Netzwerk{		//Nice and simple
   submodules:
        Sender: Node;
        Receiver: Node;
        
    connections:
        Sender.port <--> {  delay = 100ms; } <--> Receiver.port;
}  
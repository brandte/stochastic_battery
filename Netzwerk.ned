package stochastic_battery;

network Netzwerk{		//Nice and simple
   submodules:
        Sender: Node;
        Receiver: Node;
        
    connections:
        Sender.port <--> Receiver.port;	// <--> {  delay = 1ms; }
}  
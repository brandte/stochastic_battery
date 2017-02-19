package stochastic_battery;

//Simplest network possible, consisting only sender and receiver.
//We do not consider delay. There is no additional value in it and it only increases the number of events.

network Netzwerk{		//Sorry for german terminology. Old habits die hard.
   submodules:		//Sender and Receiver are based on the same compound module, but can only serve one purpose when they are defined here.
        Sender: Node;
        Receiver: Node;
        
    connections:
        Sender.port <--> Receiver.port;	// <--> {  delay = 1ms; }
}  
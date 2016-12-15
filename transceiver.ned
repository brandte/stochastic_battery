package stochastic_battery;

simple transceiver
{
        parameters: //setting the parameters to a default value!
           	int message_length = default(500);
            int sending_interval = default(30);
            string transceiver_type = default("CC2530");
            
      	gates:
        	inout battery_connection;
        	inout transmission;
}

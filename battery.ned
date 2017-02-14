package stochastic_battery;

simple battery
{
    parameters:	//description Text
		double N=default(10^10);
		double q_0=default(0.95);
		double g_N=default(1e-10);
		double g_C=default(1e-8);
		double battery_capacity=default(108);
		
    gates:
        inout gate; 
}

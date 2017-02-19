# Stochastic Battery Model
This is an OMNeT++ simulation for a stochastic battery model based on the theory of Chiasserini and Rao.  
It was done within the course *Stochastic Simulation of Communication Networks and their Protocols* at the University of Bremen, hosted by the ComNets group of Prof. Anna Förster. The specific task can be found project_task.pdf
### What is given
This GitHub Repository contains a runnable OMNeT++ 5.0 file set.  
The preset values within the simulation are set for energy efficient radio-based devices. An example with some options is given.  
Be aware, that the original model was not intended for such a scenario, what leads to long simulation durations (up to hours)!
###Parameters
There are examples given for transceiver parameters, as well as the possibility to set own values in the omnetpp.ini. In addition you can set the size of the Battery. We recommend not to stretch them too far, because this might break the limits of the model.  
To extend it, you have to adjust the core parameters of the stochastic model. We highly recommend to look at the more comprehensive documentation in the <name.pdf here>. It was originally the report for the stated university curse, but also serves as an additional description of the project.

Best success with your own project, might our work help you.  
*Eike Brandt  
Florian Schaffarzyk*  
20 February 2017

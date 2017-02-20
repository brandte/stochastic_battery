# Stochastic Battery Model
This is an OMNeT++ simulation for a stochastic battery model based on the theory of Chiasserini and Rao. [1-4]  
It was done within the course *Stochastic Simulation of Communication Networks and their Protocols* at the University of Bremen, hosted by the ComNets group of Prof. Anna Förster. The specific task can be found project_task.pdf
### What is given
This GitHub Repository contains a runnable OMNeT++ 5.0 file set.  
The preset values within the simulation are set for energy efficient radio-based devices. An example with some options is given.  
Be aware, that the original model was not intended for such a scenario, what leads to long simulation durations (up to hours)!
###Parameters
There are examples given for transceiver parameters, as well as the possibility to set own values in the omnetpp.ini. In addition you can set the size of the Battery. We recommend not to stretch them too far, because this might break the limits of the model.  
To extend it, you have to adjust the core parameters of the stochastic model. We highly recommend to look at the more comprehensive documentation in the [name.pdf comming soon]. It was originally the report for the stated university curse, but also serves as an additional description of the project.

Best success with your own project, might our work help you.  
*Eike Brandt  
Florian Schaffarzyk*  
20 February 2017

####Sources
[1]	C.-F. Chiasserini and R. R. Rao, “A model for battery pulsed discharge with recovery effect,” in Wireless Communications and Networking Conference, 1999. WCNC. 1999 IEEE, 1999, vol. 2, pp. 636–639.  
[2]	C.-F. Chiasserini and R. R. Rao, “Pulsed battery discharge in communication devices,” in Proceedings of the 5th annual ACM/IEEE international conference on Mobile computing and networking, 1999, pp. 88–95.  
[3]	C.-F. Chiasserini and R. R. Rao, “Improving battery performance by using traffic shaping techniques,” IEEE J. Sel. Areas Commun., vol. 19, no. 7, pp. 1385–1394, 2001.  
[4]	C.-F. Chiasserini and R. R. Rao, “Energy efficient battery management,” IEEE J. Sel. Areas Commun., vol. 19, no. 7, pp. 1235–1245, 2001.

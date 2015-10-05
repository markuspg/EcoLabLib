# EcoLabLib

## Purpose

*EcoLabLib* is a library doing all work for *Labcontrol*, which functions just as graphical user interface to *EcoLabLib*. Together they shall support and simplify the conduction of economic experiments. *EcoLabLib* offers the following functionality:

* Booting and shutting down clients
* Starting and killing *z-Leaves* on the clients
* Managing one or multiple instances of *z-Tree* and their connected clients
* Automatic receipts creation and printing according to different templates
* Conducting administrative tasks on the clients
* Displaying the laboratory's webcams
* Opening *ORSEE*
* Displaying the clients' file systems in a file manager
* Beaming media files to the clients
* Receiving help messages from the clients

## Other tools and programs

* [Labcontrol](https://github.com/markuspg/Labcontrol) is the graphical user interface for *EcoLabLib*
* [ClientClient](https://github.com/markuspg/ClientClient) runs in the graphical session of the clients and tries to connect to *Labcontrol* to receive its commands
* [HelpMessageSender](https://github.com/markuspg/HelpMessageSender) can be used by the experiment's participants to send help requests to the experimenter using *Labcontrol*
* [WebcamDisplay](https://github.com/markuspg/WebcamDisplay) is used by *Labcontrol* to display the webcams of the laboratory

## Further acknowledgements

For full functionality all of the above mentioned programs should be installed and configured in the laboratory. Installation and configuration are described in detail in every tool's *doc* folder.

Currently *EcoLabLib* and the supporting programs are only known to work properly on Linux. Setups with a Linux server and Windows clients and vice versa were also tested, but only to a minimum amount. Additions and fixes (especially for the build procedure) for Windows systems are very welcome.

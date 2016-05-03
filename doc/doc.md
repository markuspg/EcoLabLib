# EcoLabLib

## Installation

Create two new directories, one to contain all files needed by *EcoLabLib* (e.g. */usr/share/ecolablib* or *C:\\EcoLabLib*) and another one to place all *z-Tree* executables in (e.g. */opt* or *C:\\zTreeExecutables*). All *z-Tree* executables have to be placed in subdirectories of the second one following the naming scheme *zTree_X.Y.Z* or *zTree_X.Y.ZZ*. Otherwise the automatic detection of the installed *z-Tree* versions will not work.

On *Linux* create the folder */etc/xdg/Economic Laboratory* and copy the *EcoLabLib.conf* file from the *data* directory there. On Windows the ini file should be placed at *C:\\EcoLabLib\\EcoLabLib.conf*. Afterwards edit all values according to your laboratory's setup. In all fields accepting multiple values, separate them using '|' (as is also seen in the example entries). All configuration variables have comments, explaining their purpose.

Automatic receipts creation relies on prefabricated LaTeX headers of a specific structure. An example which can be adjusted to each laboratory's needs is delivered in the *data* folder. Multiple headers can be supplied in the EcoLabLib installation directory. All headers have to end with *_header.tex* for the automatic detection to work correctly. It's suggested to do a test run with the example header and then modifying it step by step, until it suits the user's needs.

Some laboratories need some repeatedly printed files, e.g. language tests or blank receipts. Such files can be stored in a sub directory *preprints* in the EcoLabLib installation directory. For full functionality this folder should also be created.

On Linux it is sufficient to build this project from *Qt Creator* and afterwards run *make install* as root in the build directory. Afterwards 'Labcontrol' can be built.

Since *EcoLabLib* listens on the port configured via the *server_port* variable for incoming client connections, it should be verified, that this port is not blocked by a firewall on the network device used for client connections.

## Creating the SSL Certificate and Key Files

To create SSL certificate and key files for your laboratory run *openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout ecolablib.key -out ecolablib.cert* on an arbitrary Linux. This will create both needed files. As *Common Name* please enter the fully qualified domain name of your laboratory's server.

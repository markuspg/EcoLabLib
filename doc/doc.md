# EcoLabLib

## Installation

Create two new directories, one to contain all files needed by *EcoLabLib* (e.g. */usr/share/ecolablib*) and another one to place all *z-Tree* executables in (e.g. */opt*). All *z-Tree* executables have to be placed in sub-directories of the second one following the naming scheme *zTree_X.Y.Z* or *zTree_X.Y.ZZ*. Otherwise the automatic detection of the installed *z-Tree* versions will not work.

On *Linux* create the folder */etc/xdg/Economic Laboratory* and copy the *EcoLabLib.conf* file from the *data* directory there. Afterwards edit all values according to your laboratory's setup. In all fields accepting multiple values, separate them using '|' (as is also seen in the example entries).

Automatic receipts creation relies on prefabricated LaTeX headers of a specific structure. An example is delivered in the *data* folder. Multiple headers can be supplied in the EcoLabLib installation directory. All headers have to end with *_header.tex* for the automatic detection to work correctly. It's suggest to do a test run with the example header and then modifying it step by step, until it suits the users needs.

Some laboratories need some repeatedly printed files, e.g. language tests or blank receipts. Such files can be stored in a sub directory *preprints* in the EcoLabLib installation directory. For full functionality this folder should also be created.

On Linux it is sufficient to build this project from *Qt Creator* and afterwards run *make install* as root in the build directory.

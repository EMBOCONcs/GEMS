GEMS - Generic EMBOCON Minimal Supervisor - 2013

GEMS Specification

The GEMS - Generic EMBOCON Minimal Supervisor is designed to be a minimal-footprint implementation
of the supervisor specication. The GEMS was implemented in the EMBOCON project
oering the full support of the entire set of standard interfaces of the EMBOCON platform as well as
measuring function call computation times and logging of all data.

Te architecture of GEMS includes the four supported interfaces and the data which is
exchanged. The GEMS implementation supports all standard function call denitions of the interfaces
and some of the optinonal function call denitions.

The implementation language of GEMS is a combination of C and C++. The interfaces are linked
via shared libaries under the Linux operating system. The source code is released as open-source at
the end of the project under the MIT license using the GitHub repository system. The respository
location of GEMS is the following: https://github.com/EMBOCONcs/GEMS.git. GEMS is contained
in single projects for the Eclipse IDE.

The GEMS is used together with the state-of-the-art numerical algorithms developed within the project
to show the applicability and exibility of the software platform as well as the performance of the
optimization packages on selected applications across dierent target sectors.

Supervisor Specification

The supervisor of the online part of the EMBOCON software platform forms the communication
manager between an optimizer, an observer and an application or simulation environment of the
optimization routine. The supervisor is responsible for the initialization and freeing of the dierent
elements and for triggering the functions calls of elements in the right order. The supervisor oers
logging of data like controls, states etc. and measures computation times for all function calls. The
measured timings together with the logged data can be used for testing, validation and performance
evaluation of numerical algorithms.

The standard computation cycle of the supervisor starts with the setup of the elements of the online
part by connecting them to the supervisor. Next, the creation and initialization phase starts
by initializing the application or simulation environment and getting the rst measurements of the
current state and the parameters of the system. These measurements are used to trigger the creation
and initialization step of the observer and of the optimizer including the creation and initialization
of the connected models. In the initialization, rst control inputs and states measurements are read
from the models. After the initialization of all elements is done, the optimization cycle is executed for
a predened number of steps by rst calling the makeObsStep function of the Observer Interface using
the inital controls and state measurements. The resulting states of the makeObsStep call are passed
to the optimizer and the makeOptStep function is called. The optimizer calculates new control inputs
for the next step. The resulting control inputs of the optimizer call are passed to the simulation or
application by calling the makeSimStep function of the SimulationApplication Interface. Then new
measurements of the system response to the applied control actions are passed again to the observer
and the makeObsStep function of the Observer Interface is called to calculate the next states. This
cycle is repeated until a predened number of steps as a termination criterion is reached. During the
optimization cycle computation times for any function calls are measured and logged with all data
exchanged via the supervisor.

The supervisor is not required to read and interpret conguration les for numerical algorithms connected
to it. If conguration les turn out to be necessary for connecting a numerical algorithm or
an element of a numerical algorithm to the supervisor, the numerical algorithm implementation has
to take care of the conguration le handling.

The format of the log les of the EMBOCON supervisor is not restricted to a specic format. An
implementation is only required to log all exchanged data and time measurements of all function calls.
Other features of the supervisor like graphical user interfaces, functionalities for plotting and visualization
of results, comparison of results, providing a result and a model database including import
and export functions are optional features of single realization of a supervisor, but are not mandatory
for a valid supervisor implementation.

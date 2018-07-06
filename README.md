# Stream500 Benchmarking Reference Implementation

Functional prototype of a 1-to-1 Generator to Responder implementation of the Stream 500 benchmark.

## Description

    Stream 500 is a streaming benchmark which requires processing of data whose underlying structure can 
    be represented as a graph. As an initial exploration into benchmarking streaming graph-based data, a 
    basic graph property, reachability, is used for measuring performance in the benchmark.

    Reachability - Given two vertices of an undirected graph, u and v, u can reach v and vice versa if a 
    sequence of connected vertices, a path of edges, exists between u and v.
        For example, if the following edges exist in an undirected graph:
		(1,2) (2,3) (2,4) (5,6)
	    then, reachability status among all vertices paired with vertex 1 would be:
		1,2 - Reachable
		1,3 - Reachable
		1,4 - Reachable
		1,5 - Unreachable
		1,6 - Unreachable

    In this benchmark a graph Generator produces edges, via various graph generation mechanisms, (currently 
    Kronecker, Erdos-Enyi, and basic Power Law mechanisms are supported) that are streamed to a Responder 
    via UDP socket. The Responder must process all incoming edges and maintain reachability status among all
    received vertices. Responder uses unionfind data structure to keep track of reachability.

    Periodically, the Generator produces queries of reachability status between pairs of vertices that the 
    Responder must process and respond to immediately as either reachable or not. Responder uses TCP socket 
    to send its responses to the queries.

    The Generator guarantees at the time a query is generated that the vertices in the query are definitely 
    connected by a path of edges, or that no path exists which connects the vertices.
    Therefore, all responses from the Responder can be accurately scored and wrong responses are a result of 
    some fault of the responding process.

    The expected fault in this benchmark is the ability of the responding system to keep pace with the 
    generating process. As the Generator produces edges quicker than the Responder can process them, 
    information about the graph is lost, and the potential for maintaining incomplete information about 
    the reachability between vertices increases.

### Dependency
```
* GCC >= 4.9
* LCOV 1.12
* Java JDK 8 (only required when using twitter style obfuscation)
```

## Usage

A step by step series of steps that tells you how to get it running

To setup parameters
```
The configurationFile is available in the etc folder that allows to setup the system with user defined parameters.
View README file in etc folder for description of these operating parameters
```
To Build
```
From the top level of the project, run make all to recursively build all of the object and binary files needed.
Executables will be in the bin/ directory at the top level.
```
To Run
```
Responder must be run first.
Functionality currently only supports proper parameter parsing from a configuration file. 
Responder has completed running when output stats have printed.
  e.g. ./bin/responder -c etc/configurationFile

Generator is run after beginning the responder.
Functionality currently only supports proper parameter parsing from a configuration file.
Generator has completed running when output stats have printed.
 e.g. ./bin/generator -c etc/configurationFile
 
```

### Instruction for using Twitter Style Obfuscation
* Setup the LD_LIBRARY_PATH correctly as shown below:
```
  export LD_LIBRARY_PATH=/usr/lib/jvm/java-8-oracle/lib/amd64:/usr/lib/jvm/java-8-oracle/jre/lib/amd64/server
  ```
* Please modify it accordingly to the system. If set incorrectly, java/lang/NoClassDefFoundError error is seen.

## Running Unit Tests
To Run
```
Go to the unitTests folder and run make all in the terminal. This will generate a new folder called 
coverageOutput inside the unitTests folder. Click on the index.html file inside coverageOutput folder
to view the test coverage.
```
## Version
* Prototype (Tested on ubuntu 16.04)

## Developers
* **Thomas Green**, Student, Boise State University 
* **Enjal Parajuli**, Student, Boise State University
* **Kelsey Suyehira**, Student, Boise State University
## Contributors
* [**Dr. Richard Murphy**](https://www.richardmurphy.net/), Director of Advanced Computing Solutions Pathfinding, Micron Technology 
* [**Dr. Tim Andersen**](https://coen.boisestate.edu/faculty-staff/tandersen/), Professor, Boise State University
* [**Dr. Catherine Olschanowsky**](https://coen.boisestate.edu/faculty-staff/catherineolschan/), Assistant Professor, Boise State University
* [**Dr. Anton Korzh**](https://www.linkedin.com/in/antonkorzh), System Architect, Micron Technology

## Acknowledgements
* [Graph500 Community](https://graph500.org/)
* Sandia National Labs

## License
This project is licensed under the [NCSA](https://opensource.org/licenses/NCSA) License - see the [LICENSE](LICENSE) file for details


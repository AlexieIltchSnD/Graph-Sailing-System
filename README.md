# Global Shipping Network Simulator – Advanced OOP, Spring 2025

## Overview
This project implements a **global shipping network simulator** in C++.  
The system models cargo ships transporting containers between ports, using two interconnected graphs:  

1. **Container Graph** – Edges represent container flow; weights = number of containers transported.  
2. **Time Graph** – Edges represent average travel times; weights = minutes required for a ship to sail.

The program loads network data from files, allows dynamic updates, and provides interactive queries via a terminal interface.

---

## Features

### Graph-Based Shipping Network
- **Nodes (`Node.h`)**: Represent ports.  
- **Edges (`Edge.h`)**: Represent container flows and ship routes, storing both container counts and travel times.  
- **Graphs (`Graph.h`)**: Maintain container and time networks, updated dynamically with input data.  
- **Sail Details (`SailDetails.cpp/h`)**: Store metadata for each ship’s journey, including arrival/departure times and container counts.

### Exception Handling
- **File Exceptions (`FileException.h`)**: Handles invalid inputs, unreadable files, and incorrect formats.  
- Reports errors with file name and line number and exits gracefully or ignores problematic data during interactive updates.

### Terminal Interface (`Terminal.cpp/h` + `CommandGenerator.cpp`)
Supports user commands:  

| Command | Description |
|---------|-------------|
| `load <file>` | Load additional network data; updates graphs if valid. |
| `<port>,outbound` | List ports reachable in one hop with travel times. |
| `<port>,inbound` | List ports from which the given port can be reached in one hop. |
| `<port>,balance,<dd/mm HH:mm>` | Compute container balance at a port at a specified time. |
| `print` | Output current network graphs to the output file. |
| `exit` | Exit the terminal session. |

Invalid commands or formats trigger clear error messages:  
- USAGE: 'load' <file> or
<node>,'inbound' or
<node>,'outbound' or
<node>,'balance',dd/mm HH:mm or
'print' or
'exit' to terminate

## Input File Format
- Each input file describes one ship journey, indirectly defining the graph.  
- **First line:** Source port and departure time:  
<port_name>,<departure_time>
- **Subsequent lines:** Arrival at ports, container counts, and departure:  
<port_name>,<arrival_time>,<container_quantity>,<departure_time>
- Times are in `dd/mm HH:mm` format; container quantities are positive integers.  
- If an edge already exists in the graph, container and time weights are updated using averages.

## Example Input File:
- Tuen Mun,05/01 10:01
- Singapore,11/01 16:18,67,12/01 09:15
- Mumbai,19/01 07:45,23,19/01 20:30
- Suez,24/01 11:15,15,25/01 10:00
- Ashdod,26/01 07:00,44,29/01 08:50
---

## Project Structure
- ├── main.cpp # Entry point; parses arguments and starts terminal
- ├── Terminal.cpp/h # Handles user input and interactive commands
- ├── CommandGenerator.cpp # Parses commands and dispatches actions
- ├── Graph.h # Template for container and time graphs
- ├── Node.h # Represents ports (graph nodes)
- ├── Edge.h # Represents edges with weights (containers/time)
- ├── SailDetails.cpp/h # Stores details of each ship’s voyage
- ├── Utils.cpp/h # Utility functions (date/time parsing, string handling)
- ├── FileException.h # Custom exceptions for invalid input files

---

## Building the Project
C++11 (or later) is required; smart pointers and move semantics are used for efficiency.

### Compilation Example:
```bash
g++ -std=c++11 -Wall -Wextra -o cargoBL *.cpp
./cargoBL -i <infile1> [ <infile2> <infile3> ... ] [-o <outfile>]
```

- At least one input file is required.

- Default output file: output.dat if -o is not specified.

- Errors in initial loading terminate the program; errors during interactive updates are reported but ignored for that file.


## Example Interactive Session
[input] load HapagLloyd_3.dat
[output] Update was successful.

[input] Reykjavik,outbound
[output] Newark,10504

[input] Boston,outbound
[output] Boston: no outbound ports

[input] Tuen Mun,balance,05/01 10:01
[output] -149

[input] print
[output] Network graphs written to output file

[input] exit
[output] Exiting simulator.

---

## Notes
- Exception Handling: All invalid file data, negative travel times, or malformed lines trigger exceptions.
- Graphs: Two interconnected graphs are maintained for containers and travel times.
- Efficiency: Move semantics and smart pointers are used where possible for performance.
- Port Names: Maximum 16 characters, alphabetic and spaces.
- Compilation: Must compile without warnings on the college-provided g++ compiler.

---

## Author
Student: Alexie Ilchuk

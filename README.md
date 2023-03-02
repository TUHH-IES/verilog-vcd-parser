
# VCD Tools
This is a multipurpose utility for handling VCD files.  
It started as a fork of the excellent Verilog VCD Parser, therefore keeping its original README file below.  

## Features
* Display full path of signals contained within th VCD file.
* Display the list of scopes within the VCD file.
* Display VCD file header
* Display number of toggles for each signal
* Restrict VCD file to a range of timestamps

## TODO
* Export VCD file (useful for producing a cut-down VCD file)
* Filter some signals/scopes (useful for the VCD export)

Please see below for the original Verilog VCD Parser README.md file:

---
# Verilog VCD Parser

This project implements a no-frills *Value Change Dump* (VCD) file parser, as
described in the IEEE System Verilog 1800-2012 standard. It can be used to
write custom tools which need to read signal traces dumped out by Verilog (or
VHDL) simulators.

---

## Getting Started

Install Bison and Flex using the following command:

```sh
$> sudo apt install bison flex
```

After cloning the repository to your local machine, run the following commands in a
shell:

```sh
$> cd ./verilog-vcd-parser
$> mkdir build && cd build
$> cmake ..
$> make
```

This will build the demonstration executable in `build/vcd-demonstrator`.

## Code Example

This code will load up a VCD file and print the hierarchy of the scopes
and signals declared in it.

```cpp
VCDFileParser parser;

auto trace = parser.parse_file("path-to-my-file.vcd");

if(trace == nullptr) {
    // Something went wrong.
} else {

    for(VCDScope* scope : trace->get_scopes()) {

        std::cout << "Scope: " << scope->name  << std::endl;

        for(VCDSignal * signal : scope->signals) {

            std::cout << "\t" << signal -> hash << "\t" 
                      << signal->reference;

            if(signal -> size > 1) {
                std::cout << " [" << signal->size << ":0]";
            }
            
            std::cout << std::endl;

        }
    }

}
```

We can also query the value of a signal at a particular time. Because a VCD
file can have multiple signals in multiple scopes which represent the same
physical signal, we use the signal hash to access its value at a particular
time:

```cpp
// Get the first signal we fancy.
VCDSignal* mysignal = trace->get_scope("$root")->signals[0];

// Print the value of this signal at every time step.

for (VCDTime time : trace->get_timestamps()) {

    const VCDValue& val = trace->get_signal_value_at(mysignal->hash, time);

    std::cout << "t = " << time
              << ", "   << mysignal->reference
              << " = ";
    
    // Assumes val is not nullptr!
    switch(val.get_type()) {
        case (VCDValueType::SCALAR): {
            std::cout << VCDValue::VCDBit2Char(val.get_value_bit());
            break;
        }
        case (VCDValueType::VECTOR): {
            const VCDBitVector& vecval = val.get_value_vector();
            for (const auto& it : vecval) {
                std::cout << VCDValue::VCDBit2Char(it);
            }
            break;
        }
        case (VCDValueType::REAL): {
            std::cout << val.get_value_real();
        }
        default:
            break;
    }

    std::cout << std::endl;

}

```

The example above is deliberately verbose to show how common variables and
signal attributes can be accessed.


## Integration using CMake

If integrating this into a larger project, clone this project repository in your `lib` folder.
Use the following CMake code:

```cmake
add_subdirectory(lib/verilog-vcd-parser)
target_link_libraries(your-executable-name vcd-parser)
```

This will run flex and bison on the `.ypp` and `.l` files and put
the generated parser and lexer code in the corresponding subdirectory of your build directory.

Use:

```c++
#include <vcd-parser/VCDFileParser.hpp>
```

to include the parser. The rest of the header files are located in `include/vcd-parser`.

## Tools

- The parser and lexical analyser are written using Bison and Flex,
  respectively.
- The data structures and other functions are written using C++ 2011.
- The build system is CMake.
- The codebase is documented using Doxygen (see the original project).

# PSK Demodulation Program

This C++ program performs Phase Shift Keying (PSK) demodulation on binary data stored in a file. It takes command-line arguments to configure the modulation order, preamble, and input file.

## Features

- **Modulation Order**: Configurable PSK modulation order.
- **Preamble Handling**: Uses a binary string for preamble.
- **File Input**: Reads binary data from a file and processes it.

## Requirements

- **C++ Compiler**: g++ 14.2.0 or later
- **Standard Library**: C++20

## Compilation

To compile the program, use the following command:

```bash
g++ main.cpp -o main -std=c++20

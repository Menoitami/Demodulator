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
```
## Usage

Run the program using the following syntax:

```bash

./main <mod_order> <preamble_binary> <filename>
```

## Parameters

    <mod_order>: The modulation order (e.g., 4 for QPSK).
    <preamble_binary>: A binary string representing the preamble (e.g., 01011110).
    <filename>: The path to the binary file containing the input data (e.g., ./data.bin).

## Example

To run the program with modulation order 4, a preamble of 01011110, and an input file named data.bin, use the following command:

```bash

./main 4 01011110 ./data.bin
```


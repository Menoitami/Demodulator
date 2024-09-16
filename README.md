PSK Demodulation Program
Overview

This C++ program performs phase shift keying (PSK) demodulation. It reads complex data from a binary file, demodulates it using a provided preamble, and outputs the result. The program is designed to be compiled with g++ and uses modern C++ features.
Prerequisites

    g++ (GNU Compiler Collection) version 14.2.0 or higher
    C++20 standard
    Standard C++ libraries

Compilation

To compile the program, use the following command:

sh

g++ main.cpp -o main -std=c++20

This will produce an executable named main.
Usage

To run the program, use the following command-line syntax:

sh

./main <mod_order> <preamble_binary> <filename>

Arguments

    <mod_order>: The modulation order for PSK (e.g., 4 for QPSK, 8 for 8-PSK).
    <preamble_binary>: A binary string representing the preamble. This should be a sequence of binary digits (0s and 1s).
    <filename>: The path to the binary file containing the complex data.

Example

sh

./main 4 0101 data.bin

In this example:

    4 specifies 4-PSK modulation.
    "0101" is the binary representation of the preamble.
    data.bin is the file containing the complex data to be demodulated.

Output

The program will output the demodulated symbols in the terminal. The symbols are represented as integers, and they correspond to the demodulated data based on the modulation scheme and preamble provided.
Notes

    Ensure that the binary file (<filename>) is correctly formatted with complex data.
    The preamble binary string must match the expected format for accurate demodulation.
    This program uses complex numbers and advanced C++ features; ensure compatibility with the required standards.

License

This project is licensed under the MIT License. See the LICENSE file for details.

Feel free to adjust the details based on any additional specifics or preferences you have for your project!

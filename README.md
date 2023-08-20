# Huffman Text Compression

## Overview

This project implements Huffman text compression, a well-known algorithm for lossless data compression, using C. Huffman coding is efficient for compressing text data by assigning shorter codes to more frequent characters. This README provides instructions on how to use the tool for compression and decompression.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Algorithm Overview](#algorithm-overview)
- [Testing and Compression Ratios](#testing-and-compression-ratios)

# Installation

1. Clone the repository:

   ```shell
   git clone https://github.com/ante1404/Text-Compression-Tool.git Compression Tool
   cd Compression Tool
# Usage
To use the Huffman Text Compression tool, follow these steps:

## 1. Compilation
Compile the tool using the provided Makefile:
 
        make

## 2. Data Preparation
To prepare the necessary data for compression, run the following command. This step collects the letters and their frequencies from the input file:

        ./write (path-to-input-file) (path-to-output-data-file.bin)

## 3. Compression
To compress a file, use the following command:

       ./compress (path-to-input-file) (path-to-output-data-file.bin) (path-to-compressed-file.bin)

## 4. Decompression
To decompress a file, run:

        ./decompress (path-to-uncompressed-file) (path-to-output-data-file.bin) (path-to-compressed-file.bin)

Arguments with the .bin extension are non-optional, meaning they have to be binary files.

# Algorithm Overview
The Huffman algorithm implementation follows these key steps:
The Huffman algorithm is employed in this tool to achieve lossless data compression. It follows a series of steps to compress and decompress text data efficiently:

## Frequency Analysis:
The process starts with an analysis of the input text to determine the frequency of each character (e.g., letters, symbols). This step involves scanning the text and counting the occurrences of each character.

## Priority Queue:
 A priority queue is constructed using the character frequencies obtained in the previous step. The characters with higher frequencies are given higher priority. This priority queue serves as the basis for building the Huffman tree.

## Huffman Tree Construction:
The Huffman tree is constructed by repeatedly removing the two characters with the lowest frequencies from the priority queue and creating a new node with a frequency equal to the sum of their frequencies. This process continues until only one node remains, which becomes the root of the Huffman tree.

## Assigning Binary Codes:
The Huffman tree is traversed to assign binary codes to each character. During traversal, moving to the left child node corresponds to appending a '0' to the code, while moving to the right child node appends a '1'. Each character in the tree is associated with a unique binary code based on its position in the tree.

## Data Encoding:
With the Huffman tree constructed and the binary codes assigned, the input text is encoded by replacing each character with its corresponding Huffman code. This results in a binary representation of the original text, which is more compact and efficient.

## Writing to Binary File:
The Huffman-encoded data, along with additional information such as the Huffman tree structure and character frequencies, is written to a binary file. This file contains all the necessary data for decompression.

## Compression:
To compress a file, the tool reads the input file and applies the Huffman encoding based on the Huffman tree generated during compression. The encoded data is written to an output file.

## Decompression:
Decompression is achieved by reading the Huffman-encoded binary file along with the associated Huffman tree and frequencies. The Huffman tree is reconstructed from this information. The encoded data is then decoded using the tree to recover the original text.

# Testing and Compression Ratios
The tool has undergone rigorous testing with various file sizes, ranging from small kilobyte-sized files to substantial files nearing 3 gigabytes. The achieved compression ratio depends on factor such as frequency of character repetition within the text.

During testing, the compression ratio has been observed to range from approximately 1.8 (equivalent to a 46% reduction in file size) to 2.5 (equivalent to a 60% reduction in file size). This means that, on average, the tool reduces the file size to roughly half or less of its original size while preserving all the original data.

These results demonstrate the effectiveness and efficiency of the Huffman text compression algorithm implemented in this tool.


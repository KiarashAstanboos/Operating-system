# OS Project File Analyzer

This repository contains a program for analyzing files within a directory and its subdirectories. The program provides functionalities to calculate various statistics such as the total number of each type of file, maximum size, minimum size, and total size of the chosen directory and its subdirectories. It also includes graphical output implemented with GTK for a user-friendly experience.

## Features

- **Directory Selection**: Simply run the code and choose the directory you want to analyze.
- **File Statistics**: Obtain detailed statistics including the total number of each type of file, maximum size, minimum size, and total size of the directory and its subdirectories.
- **Size Calculation**: The program includes a function to convert file sizes into human-readable formats (e.g., 1.23 MB).
- **Recursive File Analysis**: Utilizes recursive file traversal to analyze files in the chosen directory and its subdirectories.
- **Thread-Safe Updates**: Ensures synchronization using mutex for updating statistics in a thread-safe manner.

## Functions

- `calculateSize`: Converts file sizes into human-readable formats.
- `find_size`: Determines the size of a file in bytes.
- `recursive_find_files`: Recursively traverses directories, analyzing files and updating statistics.
- `thread_func`: Entry point for a thread, initializes file statistics structure, analyzes files, and exits the thread.

## Graphical Output

The program provides graphical output implemented with GTK for a visually appealing and user-friendly experience.

## How to Use

1. Clone the repository to your local machine.
2. Compile and run the code.
3. Choose the directory you want to analyze.
4. View the detailed statistics and graphical output.

## Dependencies

- GTK: Graphical output is implemented using GTK. Ensure you have GTK installed on your system.

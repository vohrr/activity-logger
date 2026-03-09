# **Activity Logger**

#### Activity Logger is a simple application that provides a journal-entry style GUI 
#### for tracking and/or logging anything you want. Written in C using GTK-4 for the GUI.

## Prerequisites

To compile and run this application, you will need to have the following installed on your system:

- **GCC** (or any compatible C99 compiler)
- **Make**
- **pkg-config**
- **GTK4 Development Libraries** (e.g., `libgtk-4-dev` on Debian/Ubuntu, `gtk4-devel` on Fedora, or equivalent for your distribution)

## Installation & Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/activity_logger.git
   cd activity_logger
   ```

2. Build the application:
   Run the following command in the project root to compile the source code:
   ```bash
   make
   ```

3. Configuration:
   The application requires no external configuration files to run. It will default to creating logs directly where the executable is run or in its predefined data directories. 

## Running the Application

Once built, you can run the application directly from the terminal:

```bash
./activity_logger
```

## Available Commands

Here are a few `make` commands you can run for development and setup:

- `make`: Compiles the source code and creates the `activity_logger` executable.
- `make clean`: Removes the compiled executable and cleans up the build directory.
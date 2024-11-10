# Simple TCP Server with Client Identification using File Descriptors

This repository contains a simple C program that demonstrates how a server can handle multiple client connections and identify each client using file descriptors (FDs). Each client that connects to the server is assigned a unique FD by the operating system, allowing the server to distinguish between clients during communication.

## Table of Contents
- [Requirements](#requirements)
- [Code Explanation](#code-explanation)
- [Usage](#usage)
- [How it Works](#how-it-works)

## Requirements
- **GCC Compiler**: To compile the C code.
- **Basic Networking Knowledge**: Knowledge of TCP/IP and socket programming is recommended.

## Code Explanation
This program creates a simple TCP server that:
1. Listens for incoming client connections on a specified port.
2. Accepts client connections and assigns each a unique FD.
3. Reads data from each client connection and prints the client's FD and received message.

### Core Functionality
- **File Descriptor**: Used to identify each client connection. The server uses the `accept()` system call to assign a unique FD for each client connection.
- **Socket API**: The program uses socket-related functions (`socket()`, `bind()`, `listen()`, `accept()`, `read()`, and `close()`) to manage client-server connections.

## Usage
1. **Move Server Directory**:
    ```bash
    cd server
    ```

2. **Compile the program**:
    ```bash
    javac -h src.main/resources/native src/main/java/com/study/network/CustomSocketServer.java
    gcc -shared -fpic -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin -o src/main/resources/native/libcustomsocket.dylib src/c/customsocket.c
    ```

3. **Run the server**:
    ```bash
    java -Djava.library.path=src/main/resources/native -cp src/main/java com.study.network.CustomSocketServer
    ```

1. **Move to the client directory**:
    ```bash
    cd client
    ```
2. **Connect a client** (for example, using `telnet` or a Python script):
    - Open a new terminal window and connect to the server:
      ```bash
      pyton3 socket-connect.py
      ```
    - Type a message and press Enter. The server will echo the message and display the client's FD.

## How it Works
1. **Start Server**: The server initializes and listens on port `8080`.
2. **Client Connections**: When a client connects, the server accepts the connection using `accept()` and assigns a unique FD.
3. **Message Handling**: The server reads messages sent by the client on that FD, prints the message along with the FD, and then closes the connection.
4. **Identification via FD**: The server distinguishes each client by the FD value, allowing it to handle multiple clients.

Each client session will print output similar to:
```plaintext
New client connected with FD: 5
Received message from client FD 5: Hello, Server!
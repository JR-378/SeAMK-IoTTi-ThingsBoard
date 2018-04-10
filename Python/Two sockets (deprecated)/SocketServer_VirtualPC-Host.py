""" Creates a socket and waits Arduino to connnect to it and send data. It then connects to a socket in 
a virtual machine and forwards the data to it. """
import socket
import time

SOCKET_HOST = "XXX.XX.XXX.XX" # Use your computer's IP.
SOCKET_PORT = 30010           # Use this port or choose another one that is free.

def receive_Data(socket_connection):
    """ To receive data from client. """
    socket_data = socket_connection.recv(2048) # receive up to buffersize bytes from the socket
    socket_data = socket_data.decode('utf-8') # decode the bytes into string
    return socket_data

def waitForConnection(socket_server):
    """ Used to wait for a client to connect. """
    socket_connection, socket_address = socket_server.accept() # wait for an incoming connection
    print(" ** Connected with " + socket_address[0] + ":" + str(socket_address[1]))
    return socket_connection

def send_data(data_to_send):
    """ Sends data forward to another socket"""
    SOCKET_LINUX_HOST = "192.168.56.102" # Linux Server socket - use the ip in your code that runs on Linux!
    SOCKET_LINUX_PORT = 30020            # Socket port - use the port in your code that runs on Linux!

    socket_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        socket_client.connect((SOCKET_LINUX_HOST, SOCKET_LINUX_PORT)) # Connect to socket
        socket_client.send(str.encode(data_to_send)) # Send data.
        socket_client.close() # Close the connection.
        print(" *** Sent data to socket at: " + SOCKET_LINUX_HOST + ":" + str(SOCKET_LINUX_PORT))
    except ConnectionRefusedError:
        print("ERROR: Could not send data to a socket.")

def main():
    """ Creates a socket that waits for a client to connect. The client sends data and the socket
    forwards it to another socket """

    socket_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Create a socket server.
    socket_server.bind((SOCKET_HOST, SOCKET_PORT)) # Bind the socket to a local address.
    socket_server.listen(5) # Enable the server to accept connections.
    print(" * Running on http://" + SOCKET_HOST + ":" + str(SOCKET_PORT))

    while True:
        socket_connection = waitForConnection(socket_server)
        socket_data = receive_Data(socket_connection)
        if len(socket_data) > 0:
            print("-->", socket_data)
            send_data(socket_data)

if __name__ == "__main__":
    main()

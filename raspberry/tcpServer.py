import socket
import serial
import sys, getopt

def Main():
    host = ''#"127.0.0.1"
    #host = socket.socket()
    portt = 5000
    port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=3.0)
#    print (host)
    port.write("p jazda 0 0 servo 4 0 k \n")
    mySocket = socket.socket()
    mySocket.bind((host, portt))
    while True:
        mySocket.listen(1)
        conn, addr = mySocket.accept()
#        print("Connection from: " + str(addr))
        while True:
            data = conn.recv(1024).decode()
            if not data:
                break
#            print("from connected  user: " + str(data))
            port.write(str(data))
            rcv = port.readline()

            #data = str(data).upper()
#            print("sending: " + rcv )
            try:
            	conn.send(rcv.encode())
            except:
            	pass
#        print("dis conectet")
	port.write("p jazda 0 0 servo 4 0 k \n") 
    conn.close()


if __name__ == '__main__':
    Main()

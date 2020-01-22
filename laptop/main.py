import socket
import cv2
import sys, getopt

def Maina():
    #host = '192.168.43.245'
    host = '192.168.1.115'
    port = 5000
    mySocket = socket.socket()
    mySocket.connect((host, port))
    cv2.namedWindow('plane')
    key = ord('z')
    spedMax = 50
    kont = 0

    while key != ord('q'):
        forth=0
        rigth=0

        if key == 81:
            rigth = -1
        if key == 82:
            forth = 1
        if key == 83:
            rigth = 1
        if key == 84:
            forth = -1


        if key == 65361:
            rigth = -1
        if key == 65362:
            forth = 1
        if key == 65363:
            rigth = 1
        if key == 65364:
            forth = -1

        if key == ord('j'):
            rigth = -1
        if key == ord('i'):
            forth = 1
        if key == ord('l'):
            rigth = 1
        if key == ord('k'):
            forth = -1

        if key ==ord('a'):
            kont=kont+5
            if kont>90:
                kont=90
        if key ==ord('d'):
            kont=kont-5
            if kont<-90:
                kont=-90
        if key == ord('w'):
            spedMax = spedMax + 5
        if key == ord('s'):
            spedMax = spedMax - 5

        forth=forth*spedMax
        rigth=rigth*spedMax
        wiad = "p "
        wiad = wiad + "jazda " + str(forth) + " " + str(rigth) + " "
        wiad = wiad + "servo " + '4' + " " + str(kont) + " "
        wiad = wiad + "k \n"





        #print(key)
        mySocket.send(wiad.encode())
        data = mySocket.recv(1024).decode()

        print('Received from server: ' + data)

        key = cv2.waitKeyEx(300)


    mySocket.close()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    Maina()

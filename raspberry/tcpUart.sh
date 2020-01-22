sudo gpio -g mode 3 out 
sudo gpio -g write 3 0
sleep 0.1
python2.7 /home/pi/raspberryGPIO/tcpServer.py

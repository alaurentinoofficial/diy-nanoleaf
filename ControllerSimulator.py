import io
import re
import time
import serial
from threading import Thread

running = True

def receiving(ser):
    global running

    while running:
        if ser.inWaiting() > 0:
            data = ser.readline()

            if data != '':
                print(data)

if __name__ ==  '__main__':
    try:
        ser = serial.Serial(
            port="COM3",
            baudrate=115200,
            bytesize=8,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=5,
        )

        #Thread(target=receiving, args=(ser,)).start()
        
        ser.write(b'\x00<')
        print(ser.readline())

        ser.write(b'\x01\x00\xf2<')
        print(ser.readline())
        
        ser.write(b'\x03\xf2\x01\x00<')
        ser.flushInput()
        ser.flushOutput()
        print(ser.readline())

        time.sleep(1)

        ser.write(b'\x03\xf2\x01\x01<')
        ser.flushInput()
        ser.flushOutput()
        print(ser.readline())

        time.sleep(1)
        
        ser.write(b'\x03\xf2\x01\x00<')
        ser.flushInput()
        ser.flushOutput()
        print(ser.readline())

        time.sleep(1)

        ser.write(b'\x03\xf2\x01\x00<')
        ser.flushInput()
        ser.flushOutput()
        print(ser.readline())
    except:
        print("Error")
        running = False
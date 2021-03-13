import io
import time
import serial
from threading import Thread

running = True

def receiving(ser):
    global running

    while running:
        # last_received = ser.readline()
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
            timeout=0.1,
            xonxoff=0,
            rtscts=0,
            interCharTimeout=None
        )

        Thread(target=receiving, args=(ser,)).start()

        while(True):
            print()
            ser.write(b'\x01\x00\xf2<')

            # while running:
            #     if ser.inWaiting() > 0:
            #         data = ser.readline()

            #         if data != '':
            #             print(data)
            time.sleep(2)
    except:
        running = False
import threading

import serial as ser
import time
def main():
    s = ser.Serial('COM6', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)   # timeout of 1 sec so that the read and write operations are blocking,
                                # when the timeout expires the program will continue
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()



    menu = ["please choose an action:",
            "1. Blink RGB LED, color by color with delay of X[ms]",
            "2. Counting up onto LCD screen with delay of X[ms]",
            "3. Circular tone series via Buzzer with delay of X[ms]",
            "4. Get delay time X[ms]:",
            "5. Potentiometer 3-digit value [v] onto LCD",
            "6. Clear LCD screen",
            "7. Get strings from PC side and print them onto LCD",
            "8. show menu",
            "9. sleep"]

    for i in menu:
        print(i)

    state4ind = 0
    state7ind = 0




        # TX
    while 1:
        txMsg = input().strip()
        if not txMsg: continue
        if txMsg == '8':
            for i in menu:
                print(i)
            continue
        if txMsg == '4':
            state4ind = 1
        if txMsg == '7':
            state7ind = 1
        s.write(bytes(txMsg + '\n', 'ascii'))
        time.sleep(0.25)  # Delay for accurate read/write operations
        if state7ind and s.out_waiting==0:
            printstring= input("Enter Strings: ")
            s.write(bytes(printstring + '\n', 'ascii'))
            time.sleep(0.25)  # Delay for accurate read/write operations
            state7ind=False
        if state4ind and s.out_waiting == 0:
            while True:
                setX = input("Set X in ms: ")
                if setX.isdigit():
                    break
                else:
                    print("Please enter a valid number.")
            s.write(bytes(setX + '\n', 'ascii'))
            time.sleep(0.25)  # Delay for accurate read/write operations
            state4ind = False

def set_x_value(serial_connection):
    while True:
        setX = input("Set X in ms: ")
        if setX.isdigit():
            break
        else:
            print("Please enter a valid number.")
    serial_connection.write(bytes(setX + '\n', 'ascii'))
    time.sleep(0.25)  # Delay for accurate read/write operations
if __name__ == '__main__':
    main()

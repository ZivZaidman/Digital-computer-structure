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


    menu = ["please choose an action:", "1- RGB light change", "2- count up", "3- tone series", "4- set X in ms", "5- LDR measure",
                    "6- LCD clear, rest count up register to 0", "7- show menu", "8- go to sleep"]

    for i in menu:
        print(i)

    pos4 = 0
        # TX
    while 1:
        txMsg = input()
        if txMsg == '': continue
        if txMsg == '7':
            for i in menu:
                print(i)
            continue
        if txMsg == '4':
            pos4 = True
        bytetxMsg = bytes(txMsg + '\n', 'ascii')
        s.write(bytetxMsg)
        time.sleep(0.25)  # delay for accurate read/write operations on both ends
        if s.out_waiting == 0 and pos4:
            enablePos4 = True
            setX = input("set x in ms: ")
            while s.out_waiting > 0 or enablePos4:
                if setX == '':
                    setX = input("please enter a number to set x in ms")
                bytetxX = bytes(setX + '\n', 'ascii')
                s.write(bytetxX)
                time.sleep(0.25)  # delay for accurate read/write operations on both ends
                if s.out_waiting == 0:
                    enablePos4 = False
            pos4 = False


if __name__ == '__main__':
    main()

import PySimpleGUI as sg
import serial as ser
import sys, glob
import time
import serial.tools.list_ports
from tkinter import *
from tkinter.colorchooser import askcolor
import mouse
import os
from os import path
import threading
import binascii
import math
import pyautogui
#---------------GUI definition--------------#
#----Main window-----#
sg.theme('DarkGrey9')
layout=[[sg.Button("calibaration", size=(30,4),button_color=("white", "green"), font=("Helvetica", 14))
            , sg.Button("Joystick Controled Rotation", size=(30,4),button_color=("white", "green"), font=("Helvetica", 14))],
        [sg.Button("Joystick Painter", size=(30,4),button_color=("white", "green"), font=("Helvetica", 14))
            ,sg.Button("Script",size=(30,4),button_color=("white", "green"), font=("Helvetica", 14))],
        [sg.Exit(size=(61,2),button_color=("white", "darkred"), font=("Helvetica", 14))]]

window = sg.Window("Final project Menu", layout)
#-----UARTconfig-----#
s = ser.Serial('COM5', baudrate=9600, bytesize=ser.EIGHTBITS,
               parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
               timeout=1)  # timeout of 1 sec so that the read and write operations are blocking,
# when the timeout expires the program will continue
# clear buffers
s.reset_input_buffer()
s.reset_output_buffer()
#----Calibration window-----#
RotationCounter=0
degreesCounter=0
def open_calibration_window(i=None):
    s.reset_input_buffer()
    s.write(bytes('C' + '\n', 'ascii'))
    time.sleep(0.25)  # Delay for accurate read/write operations
    s.reset_output_buffer()
    received_int = [0,0,0,0,0]
    i=0
    while(i<5):
        if s.in_waiting > 0:
            received_data = s.read(1)
            received_int[i] = int.from_bytes(received_data, byteorder='little')
            print("received_data: ", received_data)
            print("received_int: ", received_int)
            s.reset_input_buffer()
            i+=1
        number_of_rotations = int(''.join(map(str, received_int)))
    i=0
    while (i < 5):
        if s.in_waiting > 0:
            received_data = s.read(1)
            received_int[i] = int.from_bytes(received_data, byteorder='little')
            print("received_data: ", received_data)
            print("received_int: ", received_int)
            s.reset_input_buffer()
            i += 1
        degreesCounter = int(''.join(map(str, received_int)))
    """while True:
        if s.in_waiting>0:
            received_data = s.read_until(expected=b'\n')
            #received_data = s.readline()
            received_int=int.from_bytes(received_data,byteorder='little')*(256)
            print("received_data: ", received_data)
            print("received_int: ",received_int)
            s.reset_input_buffer()
            break
    while True:
        if s.in_waiting>0:
            received_data = s.read_until(expected=b'\n')
            received_int=received_int+int.from_bytes(received_data,byteorder='little')
            print("received_data2: ", received_data)
            print("received_int2: ",received_int)
            s.reset_input_buffer()
            break"""
            #    time.sleep(0.25)  # Delay for accurate read/write operations
            #if s.in_waiting > 0:
    """while True:
        if s.in_waiting>0:
            received_data = s.read_until(expected=b'\n')
            degreesCounter=int.from_bytes(received_data,byteorder='little')
            s.reset_input_buffer()
            print("degreesCounter",degreesCounter)
            break"""
     #   time.sleep(0.25)  # Delay for accurate read/write operations
            # Process the received data here
    calibration_layout = [[sg.Text("Rotations Counter:"), sg.Text(number_of_rotations)],
                          [[sg.Text("degrees:"), sg.Text(degreesCounter)]],
                          [sg.Exit(size=(15, 2), button_color=("white", "green"), font=("Helvetica", 14))]]

    calibration_window = sg.Window("Calibaration", calibration_layout)

    while True:
        calibration_event, _ = calibration_window.read()
        if (calibration_event in (sg.WIN_CLOSED, "Exit")):
            s.write(bytes('I' + '\n', 'ascii'))
            time.sleep(0.25)  # Delay for accurate read/write operations
            s.reset_output_buffer()
            break
    calibration_window.close()
def opem_JoystickRotation_window():
    s.write(bytes('J' + '\n', 'ascii'))
    time.sleep(0.25)  # Delay for accurate read/write operations
    s.reset_output_buffer()
    joystickRotation_layout = [[sg.Text("Use your Joystick to rotate the stepmotor")],
                          [sg.Exit(size=(15, 2), button_color=("white", "green"), font=("Helvetica", 14))]]
    joystickRotation_window=sg.Window("joystickRotation",joystickRotation_layout)
    while True:
        joystickRotation_event,_=joystickRotation_window.read()
        if (joystickRotation_event in (sg.WIN_CLOSED, "Exit")):
            s.write(bytes('I' + '\n', 'ascii'))
            time.sleep(0.25)  # Delay for accurate read/write operations
            s.reset_output_buffer()
            break
    joystickRotation_window.close()

def open_canvas():
    s.reset_input_buffer()
    s.write(bytes('P' + '\n', 'ascii'))
    time.sleep(0.25)  # Delay for accurate read/write operations
    s.reset_output_buffer()
    modes = ['Draw', 'Erase', 'Neutral']  # Available modes
    current_mode_index = 0  # Start with 'Draw' mode
    Canvas_layout = [
        [sg.Graph(
            canvas_size=(600, 600),
            graph_bottom_left=(0, 0),
            graph_top_right=(600, 600),
            background_color='white',
            key='-GRAPH-',
            change_submits=True,  # Submit events on changes (like clicks)
            drag_submits=False,  # Don't submit drag events
        )],
        [sg.Text(f'Mode: {modes[current_mode_index]}',key='mode_text'),
         sg.Button('Clear', size=(10, 1), button_color=("white", "green")),
         sg.Button('Exit', size=(10, 1), button_color=("white", "green"))]
    ]

    painter_window = sg.Window('Joystick Painter', Canvas_layout, finalize=True)
    graph = painter_window['-GRAPH-']
    tk_canvas = graph.Widget
    prev_pos = (300,300)
    neutral_item=None
    erase_item=None

    def calculate_xy(angle, radius=15):
        radian = math.radians(angle)
        # Calculate new x and y positions using polar coordinates
        #x = radius * math.cos(radian) + 600 // 2
        #y = radius * math.sin(radian) + 600 // 2
        x = radius * math.cos(radian)
        y = radius * math.sin(radian)
        return int(x), int(y)

    def move_brush():
        i = 0
        received_angle = [0, 0, 0, 0, 0]
        s.write(bytes('A' + '\n', 'ascii'))
        print("A")
        time.sleep(0.25)
        s.reset_output_buffer()  # send ACK, wait for angle and than move acordingly

        while (i < 5):
            #print(i)
            if s.in_waiting > 0:
                received_data = s.read(1)
                received_angle[i] = int.from_bytes(received_data, byteorder='little')
                if received_angle[i]==0x99:
                    return
                s.reset_input_buffer()
                i += 1
        angle = int(''.join(map(str, received_angle)))
        if (angle<0 or angle>360):
            return
        #print("received_data: ", received_data)
        #print("received_int: ", received_angle)
        print("received ANGLE: ", angle)
        #tk_canvas.bind('<Motion>', move_brush()
        nonlocal prev_pos, neutral_item, erase_item

        x, y = calculate_xy(angle)
        if 0 <= x <= 600 and 0 <= y <= 600:
            current_mode = modes[current_mode_index]

            if current_mode == 'Draw':
                #if prev_pos==(0,0):
                graph.draw_line(prev_pos,  (prev_pos[0]+x, prev_pos[1]+y), color='black', width=2)
                prev_pos = (prev_pos[0]+x, prev_pos[1]+y)

            elif current_mode == 'Erase':
                #if prev_pos==(0,0):
                graph.draw_line(prev_pos,  (prev_pos[0]+x, prev_pos[1]+y), color='white', width=30)
                prev_pos = (prev_pos[0]+x, prev_pos[1]+y)
                if erase_item:
                    graph.delete_figure(erase_item)
                erase_item = graph.draw_circle( (prev_pos[0]+x, prev_pos[1]+y),radius=20,fill_color='white',line_color='black')
                prev_pos=(prev_pos[0]+x,prev_pos[1]+y)

            elif current_mode == 'Neutral':
                if neutral_item:
                    graph.delete_figure(neutral_item)
                neutral_item = graph.draw_circle( (prev_pos[0]+x, prev_pos[1]+y),radius=20,fill_color='blue',line_color='blue')
                prev_pos=(prev_pos[0]+x,prev_pos[1]+y)
        else:
            #we need to make prev pos to be the last xy that he was in it
            prev_pos = (300, 300)


    def change_mode():
        nonlocal current_mode_index, prev_pos, neutral_item,erase_item
        current_mode_index = (current_mode_index + 1) % len(modes)
        painter_window['mode_text'].update(f'Mode: {modes[current_mode_index]}')
        #prev_pos=(0,0)
        if neutral_item:
            graph.delete_figure(neutral_item)
            neutral_item = None
        if erase_item:
            graph.delete_figure((erase_item))
            erase_item=None

    #tk_canvas.bind('move', move_brush)  # Bind mouse movement
    #tk_canvas.bind('<Button-1>', click_event)  # Bind left mouse button click
    # event Loop
    while True:
        painter_event, values = painter_window.read(timeout=10)

        if painter_event in (sg.WIN_CLOSED, "Exit"):
            s.write(bytes('I' + '\n', 'ascii'))
            time.sleep(0.25)  # Delay for accurate read/write operations
            s.reset_output_buffer()
            break
        if s.in_waiting>0:
            joysstick_ISR=s.read(1)
            print("joysstick_ISR",joysstick_ISR)
            if joysstick_ISR == b'\x99':
                change_mode()
            if joysstick_ISR == b'\x11':#recived REQ
                move_brush()


                """while(joysstick_ISR!=b'\x99'):
                    received_angle = [int.from_bytes(joysstick_ISR, byteorder='little'), 0, 0, 0, 0]
                    i = 1
                    while (i < 5):
                        if s.in_waiting > 0:
                            received_data = s.read(1)
                            received_angle[i] = int.from_bytes(received_data, byteorder='little')
                            s.reset_input_buffer()
                            i += 1
                        angle = int(''.join(map(str, received_angle)))
                        print("received_data: ", received_data)
                        print("received_int: ", received_angle)
                        print("received ANGLE: ", angle)
                    move_brush(angle)"""


    painter_window.close()

def open_script_mode():
    script_layout=[[sg.Text("Choose a flie:"),sg.Input(key="IN"),sg.FileBrowse(button_color=("white", "green"))],
                   [sg.Button("Exit",button_color=("white", "darkred")),sg.Button("upload File",button_color=("white", "green"))]]
    script_window=sg.Window("Uploading a File to the MSP430", script_layout)
    while True:
        script_event,script_values=script_window.read()
        if script_event in ("Exit",sg.WIN_CLOSED):
            break
# -----main event loop-----#
while True:
    event, _ = window.read()
    if (event in (sg.WIN_CLOSED, "Exit")):
        s.write(bytes('I' + '\n', 'ascii'))
        time.sleep(0.25)  # Delay for accurate read/write operations
        s.reset_output_buffer()
        break
    if (event == "calibaration"):
        open_calibration_window()
    if (event=="Joystick Controled Rotation"):
        opem_JoystickRotation_window()
    if (event=="Joystick Painter"):
        open_canvas()
    if (event=="Script"):
        open_script_mode()
window.close()


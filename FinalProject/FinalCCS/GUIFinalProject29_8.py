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
sg.theme('TealMono')
layout=[[sg.Button("calibaration", size=(30,4),button_color=("white", "teal"), font=("Helvetica", 14))
            , sg.Button("Joystick Controled Rotation", size=(30,4),button_color=("white", "teal"), font=("Helvetica", 14))],
        [sg.Button("Joystick Painter", size=(30,4),button_color=("white", "teal"), font=("Helvetica", 14))
            ,sg.Button("Script",size=(30,4),button_color=("white", "teal"), font=("Helvetica", 14))],
        [sg.Exit(size=(61,2),button_color=("white", "darkred"), font=("Helvetica", 14))]]

window = sg.Window("Final project Menu", layout)
file_paths=[]
items=[]
content=[]
angles=[]
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
    s.write(bytes('L' + '\n', 'ascii'))
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
                          [sg.Exit(size=(15, 2), button_color=("white", "teal"), font=("Helvetica", 14))]]

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
                          [sg.Exit(size=(15, 2), button_color=("white", "teal"), font=("Helvetica", 14))]]
    joystickRotation_window=sg.Window("joystickRotation",joystickRotation_layout)
    while True:
        joystickRotation_event,_=joystickRotation_window.read()
        if (joystickRotation_event in (sg.WIN_CLOSED, "Exit")):
            s.write(bytes('I' + '\n', 'ascii'))
            time.sleep(0.25)  # Delay for accurate read/write operations
            s.reset_output_buffer()
            break
    joystickRotation_window.close()
#################################################################################################3
import PySimpleGUI as sg
import mouse
import time
import math

def open_canvas():
    s.reset_input_buffer()
    s.write(bytes('P' + '\n', 'ascii'))
    time.sleep(0.25)  # Delay for accurate read/write operations
    s.reset_output_buffer()
    print('P')
    # Initial settings
    #print("Hello world")
    canvas_size = (500, 500)  # Define the size of the canvas
    draw_color = 'black'       # Color for drawing
    erase_color = 'white'      # Color for erasing (background color)
    hover_color = 'red'        # Color for hover effect
    modes = ['Draw', 'Erase', 'Hover']  # Available modes
    current_mode_index = 0  # Start with 'Draw' mode

    # Define the layout of the window
    layout = [
        [sg.Graph(
            canvas_size,            # Size of the canvas
            (0, 0),                 # Lower left corner coordinates
            canvas_size,            # Upper right corner coordinates
            key='canvas',           # Key to reference the canvas
            change_submits=True,    # Submit events on changes (like clicks)
            drag_submits=False,     # Don't submit drag events
            background_color='white'  # Background color of the canvas
        )],
        [sg.Text(
            f'Mode: {modes[current_mode_index]}',  # Display current mode
            key='mode_text',                        # Key to update the text
            font=('Helvetica', 14)                  # Font styling
        )],
        [sg.Button("Clear",button_color=("white", "teal")),sg.Exit(button_color=("white", "darkred"))]
    ]

    # Create the PySimpleGUI painter_window
    painter_window = sg.Window('PySimpleGUI Painter', layout, finalize=True)
    canvas = painter_window['canvas']  # Get the canvas element

    # Access the underlying Tkinter Canvas for advanced event handling
    tk_canvas = canvas.Widget

    # Variables to keep track of previous positions and hover items
    prev_pos = None      # Previous mouse position for drawing lines
    neutral_item = None    # Current hover circle (if any)
    erase_item=None
    angle=0;
    def get_angle():
        nonlocal angle
        i = 0
        received_angle = [0, 0, 0, 0, 0]
        s.write(bytes('K' + '\n', 'ascii'))
        print("ACK")
        time.sleep(0.25)
        s.reset_output_buffer()  # send ACK, wait for angle and than move acordingly

        while (i < 5):
            # print(i)
            if s.in_waiting > 0:
                received_data = s.read(1)
                received_angle[i] = int.from_bytes(received_data, byteorder='little')
                print(received_angle)
                if received_angle[i] == 0x99:
                    s.reset_input_buffer()
                    print("angle 0x99")
                    return int(0x99)
                s.reset_input_buffer()
                i += 1
        return int(''.join(map(str, received_angle)))

    def calculate_xy(angle,radius=10):

        radian = math.radians(angle)
        # Calculate new x and y positions using polar coordinates
        x = radius * math.cos(radian)
        y = radius * math.sin(radian)
        return int(x), int(y)

    # Define the motion event handler
    def motion_event(event):
        nonlocal current_mode_index, prev_pos, neutral_item, erase_item

        # Get mouse position within the canvas
        x = event.x
        y = canvas_size[1] - event.y  # Invert y-axis to match Graph coordinates

        # Check if the mouse is within the canvas boundaries
        if 0 <= x <= canvas_size[0] and 0 <= y <= canvas_size[1]:
            current_mode = modes[current_mode_index]

            if current_mode == 'Draw':
                if prev_pos:
                    # Draw a line from the previous position to the current position
                    canvas.draw_line(prev_pos, (x, y), color=draw_color, width=2)
                # Update the previous position
                prev_pos = (x, y)

            elif current_mode == 'Erase':
                if prev_pos:
                    # Erase by drawing a thick white line
                    canvas.draw_line(prev_pos, (x, y), color=erase_color, width=20)
                if erase_item:
                    canvas.delete_figure(erase_item)
                # Draw a new hover circle at the current position
                erase_item = canvas.draw_circle(
                    (x, y),
                    radius=10,
                    fill_color=erase_color,
                    line_color='black'
                )
                # Update the previous position
                prev_pos = (x, y)

            elif current_mode == 'Hover':
                # Remove the previous hover circle to avoid clutter
                if neutral_item:
                    canvas.delete_figure(neutral_item)
                # Draw a new hover circle at the current position
                neutral_item = canvas.draw_circle(
                    (x, y),
                    radius=10,
                    fill_color=hover_color,
                    line_color=hover_color
                )
                prev_pos = (x, y)

    """    # Define the click event handler for mode switching
    def click_event(event):
        nonlocal current_mode_index, prev_pos, neutral_item
        # Cycle through the modes
        current_mode_index = (current_mode_index + 1) % len(modes)
        # Update the mode display text
        window['mode_text'].update(f'Mode: {modes[current_mode_index]}')
        # Reset tracking variables
        prev_pos = None
        # Remove any existing hover circle when switching modes
        if erase_item:
            canvas.delete_figure(erase_item)
            erase_item= None
        if neutral_item:
            canvas.delete_figure(neutral_item)
            neutral_item = None"""

    def change_mode(event):
        nonlocal current_mode_index, prev_pos, neutral_item, erase_item

        current_mode_index = (current_mode_index + 1) % len(modes)
        painter_window['mode_text'].update(f'Mode: {modes[current_mode_index]}')
        #prev_pos=(0,0)
        if neutral_item:
            canvas.delete_figure(neutral_item)
            neutral_item = None
        if erase_item:
            canvas.delete_figure(erase_item)
            erase_item=None

    # Bind the motion and click events to the Tkinter Canvas
    tk_canvas.bind('<Motion>', motion_event)      # Bind mouse movement
    tk_canvas.bind('<Button-1>', change_mode)     # Bind left mouse button click
    x=0
    y=0
    # Event loop to keep the window active
    while True:
        event, values = painter_window.read(timeout=100)  # Read events with a timeout

        if event in (sg.WIN_CLOSED, "Exit"):
            break  # Exit the loop if the window is closed
        if event== "Clear":
            canvas.erase()
        if s.in_waiting>0:
            joysstick_ISR=s.read(1)
            print("joysstick_ISR",joysstick_ISR)
            if joysstick_ISR == b'\x99':
                tk_canvas.event_generate('<Button-1>')
            if joysstick_ISR == b'\x11':#recived REQ
                angle= get_angle()
                if angle==0x99:
                    tk_canvas.event_generate('<Button-1>')
                    continue
                print(angle)
                (x, y)=calculate_xy(angle)
                mouse.move(x, y, absolute=False, duration=0.2)
                x_canvas = tk_canvas.winfo_pointerx() - tk_canvas.winfo_rootx()
                y_canvas = tk_canvas.winfo_pointery() - tk_canvas.winfo_rooty()
                tk_canvas.event_generate('<Motion>', x=x_canvas, y=y_canvas)

    painter_window.close()  # Close the window gracefully

import pyautogui
import math

"""def move_in_angle(angle_degrees, step_size=1):
    angle_radians = math.radians(angle_degrees)

    delta_x = step_size * math.cos(angle_radians)
    delta_y = step_size * math.sin(angle_radians)

    sg.moveRel(delta_x, delta_y)"""
#######################################################################################################################################################################################################################3
def open_script_mode():
    global file_paths,items,content,angles
    wait_for_data=0
    s.write(bytes('S' + '\n', 'ascii'))
    time.sleep(0.25)  # Delay for accurate read/write operations
    s.reset_output_buffer()

    script_layout=[[sg.Text("upload a flie:"),sg.Input(key="IN"),sg.FileBrowse(button_color=("white", "teal"))],
                   [sg.Text("choose a flie:"),sg.Listbox(values=items, size=(30, 6), select_mode=sg.LISTBOX_SELECT_MODE_SINGLE, key='-LIST-', enable_events=True)
                       ,sg.Text("content:"),sg.Listbox(values=content, size=(30, 6), key='-CONTENT-')],
                   [sg.Button("Exit",button_color=("white", "darkred")),sg.Button("upload File",button_color=("white", "teal")),sg.Button("execute",button_color=("white", "teal")),sg.Text("dinamic angle: "),
                    sg.Text(" ", size=(10, 1), key='dangle')]]
    script_window=sg.Window("Uploading a File to the MSP430", script_layout)


    def convertfile(file_path): #####convert python comand a file to op code of hanan
        with open(file_path, 'rb') as f:
            FileContent=f.read()
        op_codes_dict={
            'inc_lcd'       :   '01',
            'dec_lcd'       :   '02',
            'rra_lcd'       :   '03',
            'set_delay'     :   '04',
            'clear_lcd'     :   '05',
            'stepper_deg'   :   '06',
            'stepper_scan'  :   '07',
            'sleep'         :   '08',
        }
        converted_lines=[]
        data=FileContent.decode('utf-8').strip()       #raw data decoded from ascii
        lines=data.split('\r\n')                        #raw data split into lines
        for line in lines:                              #find all comands, iterrations for each line
            line_part=line.split()
            comand=line_part[0]                         #command will be first

            if comand in op_codes_dict:
                op_code=op_codes_dict[comand]           #this is the translated op code for this line
                #now we will check for parts:
                if len(line_part)>1:
                    iter=line_part[1]
                    if ',' in iter:                     #there is more than one iteration data
                        iter_val=iter.split(',')
                        formatted_iter = ''.join([f'{int(d):02X}' for d in iter_val])
                    else:                               #only one iteration
                        if op_code=='03':
                            formatted_iter = f'{ord(iter):02X}'
                        else:
                            formatted_iter = f'{int(iter):02X}'
                    converted_line = op_code + formatted_iter
                else:
                    converted_line = op_code
            converted_lines.append(converted_line)                  #


        # Write the translated lines to a new file
        converted_file_path = 'coverted_file.txt'
        with open(converted_file_path, 'w') as f:
            for line in converted_lines:
                f.write(line + '\n')

        return converted_file_path  # Return the file path

    def transmit(message=None):
        with open(message, 'r') as f:
                sendmessage=f.read()
        # Transmit a message to the serial port
        s.reset_output_buffer()  # Clear the output buffer
        bytesMenu = bytes(sendmessage, 'ascii')  # Convert message to bytes
        print(bytesMenu)
        s.write(bytesMenu)  # Send the message through the serial port
        time.sleep(0.25)
        s.reset_output_buffer()  # Clear the output buffer

    while True:
        script_event,script_values=script_window.read(timeout=100)
        script_window['-CONTENT-'].update(values=content)
        if script_event in ("Exit",sg.WIN_CLOSED):
            s.write(bytes('I' + '\n', 'ascii'))
            time.sleep(0.25)  # Delay for accurate read/write operations
            s.reset_output_buffer()
            break
        if script_event == "-LIST-":
            selected_file_name = script_values['-LIST-'][0] if script_values['-LIST-'] else None
            if selected_file_name:
                selected_file_path = next((path for path in file_paths if os.path.basename(path) == selected_file_name),None)
                if selected_file_path:
                    try:
                        with open(selected_file_path, 'r') as f:
                            content = f.readlines()
                        script_window['-CONTENT-'].update(values=content)
                    except Exception as e:
                        sg.popup_error(f"Error reading file: {e}")

        if script_event == "execute":
            chosen_file=script_values['-LIST-'][0] if script_values['-LIST-'] else None
            if items[0]==chosen_file:
                s.write(bytes('Z' + '\n', 'ascii'))
                time.sleep(0.25)  # Delay for accurate read/write operations
                s.reset_output_buffer()
            elif items[1] == chosen_file:
                s.write(bytes('N' + '\n', 'ascii'))
                time.sleep(0.25)  # Delay for accurate read/write operations
                s.reset_output_buffer()
            elif items[2]==chosen_file:
                s.write(bytes('H' + '\n', 'ascii'))
                time.sleep(0.25)  # Delay for accurate read/write operations
                s.reset_output_buffer()
            """while True:
                if s.in_waiting > 0:
                    temp = s.read()
                    if temp != "22":
                        wait_for_data = 1
                if (wait_for_data):
                    data2 = s.read_until(b'\n').decode('utf-8').strip()  # angle
                    time.sleep(0.1)
                    print("data: ", data2)
                    script_window['dangle'].update(value=str(data2))  # Update the Listbox with new angle
                    s.reset_input_buffer()
                    if data2 == 'X':
                        wait_for_data = 0
                        break"""
        if script_event == "upload File":
            if len(items)==3:
                sg.popup_error("Max files uploaded, please exit and start program again.")
            file_path=script_values["IN"]            # save file path
            if file_path:
                file_paths.append(file_path)  # Add the file path to the list of files
                items=[os.path.basename(path) for path in file_paths]
                file_name=items[len(items)-1]
                file_name_bytes=bytes(file_name + "\x0a" ,'ascii')
                print("file_name_bytes: ",file_name_bytes)
                s.write(file_name_bytes)
                time.sleep(0.25)
                s.reset_output_buffer()
                s.write(bytes('M' + '\n', 'ascii'))
                time.sleep(0.25)  # Delay for accurate read/write operations
                s.reset_output_buffer()
                converted_file_path = convertfile(file_path) #converted file to op code
                transmit(converted_file_path)
                if (len(items)==1):
                    s.write(bytes('O' + '\n', 'ascii'))
                    time.sleep(0.25)  # Delay for accurate read/write operations
                    s.reset_output_buffer()
                if (len(items)==2):
                    s.write(bytes('W' + '\n', 'ascii'))
                    time.sleep(0.25)  # Delay for accurate read/write operations
                    s.reset_output_buffer()
                if (len(items)==3):
                    s.write(bytes('G' + '\n', 'ascii'))
                    time.sleep(0.25)  # Delay for accurate read/write operations
                    s.reset_output_buffer()
                while True:
                    if s.in_waiting>0:
                        recived_ack = s.read(1)
                        if recived_ack==b'\x22':
                            break
                """
                -translate file
                -send translate file
                -wait for ack
                """
                script_window['-LIST-'].update(values=items)  # Update the Listbox with new file


    script_window.close()



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


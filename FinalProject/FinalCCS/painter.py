'''import PySimpleGUI as sg

# Initial settings
canvas_size = (400, 400)
draw_color = 'black'
hover_color = 'yellow'
erase_color = 'white'
modes = ['Draw', 'Erase', 'Hover']
current_mode_index = 0  # Index to switch between modes

# Layout
layout = [
    [sg.Graph(canvas_size, (0, 0), canvas_size, key='canvas', change_submits=True, drag_submits=False,
              background_color='white')],
    [sg.Text(f'Mode: {modes[current_mode_index]}', key='mode_text')]
]

window = sg.Window('Painter', layout, finalize=True)
canvas = window['canvas']

drawing = False

while True:
    event, values = window.read(timeout=10)

    if event == sg.WIN_CLOSED:
        break

    # Switch modes by clicking the canvas
    if event == 'canvas':
        current_mode_index = (current_mode_index + 1) % len(modes)
        window['mode_text'].update(f'Mode: {modes[current_mode_index]}')
        drawing = (modes[current_mode_index] == 'Draw')

    mouse = values['canvas']
    if mouse:
        x, y = mouse
        if drawing:
            canvas.draw_point((x, y), size=5, color=draw_color)
        elif modes[current_mode_index] == 'Erase':
            canvas.draw_point((x, y), size=5, color=erase_color)
        elif modes[current_mode_index] == 'Hover':
            canvas.draw_point((x, y), size=5, color=hover_color)

window.close()'''

import PySimpleGUI as sg
import mouse
import time
import math

def main():
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
        [sg.Button("draw circle"),sg.Button("draw squere")]
    ]

    # Create the PySimpleGUI window
    window = sg.Window('PySimpleGUI Painter', layout, finalize=True)
    canvas = window['canvas']  # Get the canvas element

    # Access the underlying Tkinter Canvas for advanced event handling
    tk_canvas = canvas.Widget

    # Variables to keep track of previous positions and hover items
    prev_pos = None      # Previous mouse position for drawing lines
    hover_item = None    # Current hover circle (if any)
    erase_item=None

    # Define the motion event handler
    def motion_event(event):
        nonlocal current_mode_index, prev_pos, hover_item, erase_item

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
                if hover_item:
                    canvas.delete_figure(hover_item)
                # Draw a new hover circle at the current position
                hover_item = canvas.draw_circle(
                    (x, y),
                    radius=10,
                    fill_color=hover_color,
                    line_color=hover_color
                )

    # Define the click event handler for mode switching
    def click_event(event):
        nonlocal current_mode_index, prev_pos, hover_item
        # Cycle through the modes
        current_mode_index = (current_mode_index + 1) % len(modes)
        # Update the mode display text
        window['mode_text'].update(f'Mode: {modes[current_mode_index]}')
        # Reset tracking variables
        prev_pos = None
        # Remove any existing hover circle when switching modes
        if hover_item:
            canvas.delete_figure(hover_item)
            hover_item = None

    def draw_square(size):
        # Convert screen coordinates to canvas-relative coordinates
        for dx, dy in [(size, 0), (0, size), (-size, 0), (0, -size)]:
            mouse.move(dx, dy, absolute=False, duration=0.2)
            x_canvas = tk_canvas.winfo_pointerx() - tk_canvas.winfo_rootx()
            y_canvas = tk_canvas.winfo_pointery() - tk_canvas.winfo_rooty()
            tk_canvas.event_generate('<Motion>', x=x_canvas, y=y_canvas)

    def draw_circle(radius):
        for i in range(0, 360, 5):
            angle = math.radians(i)
            x = radius * math.cos(angle)
            y = radius * math.sin(angle)
            mouse.move(x, y, absolute=False, duration=0.01)
            x_canvas = tk_canvas.winfo_pointerx() - tk_canvas.winfo_rootx()
            y_canvas = tk_canvas.winfo_pointery() - tk_canvas.winfo_rooty()
            tk_canvas.event_generate('<Motion>', x=x_canvas, y=y_canvas)

        # release the left mouse button
        #mouse.release()
    # Bind the motion and click events to the Tkinter Canvas
    tk_canvas.bind('<Motion>', motion_event)      # Bind mouse movement
    tk_canvas.bind('<Button-1>', click_event)     # Bind left mouse button click

    # Event loop to keep the window active
    while True:
        event, values = window.read(timeout=100)  # Read events with a timeout

        if event == sg.WIN_CLOSED:
            break  # Exit the loop if the window is closed
        if event == "draw circle":
            time.sleep(1)
            draw_circle(10)
        if event== "draw squere":
            time.sleep(1)
            draw_square(100)
            #draw_square(100)
    window.close()  # Close the window gracefully

if __name__ == "__main__":
    main()

import pyautogui
import math

"""def move_in_angle(angle_degrees, step_size=1):
    angle_radians = math.radians(angle_degrees)

    delta_x = step_size * math.cos(angle_radians)
    delta_y = step_size * math.sin(angle_radians)

    sg.moveRel(delta_x, delta_y)"""

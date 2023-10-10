import tkinter as tk
from enum import Enum
import serial.tools.list_ports
root = tk.Tk()
tk.Entry(root, justify='center')
root.geometry("400x300")
root.title("Light Controller")

parameter = Enum('Frequence', ['BASS', 'MEDIUM', 'STATUS'])
is_on = False


def port_selected(event):
    print("Selected Port: ", port.get())


ports = serial.tools.list_ports.comports()

#dropdown menu to select the port
port = tk.StringVar(root)
port.set(ports[0].device)
port_menu = tk.OptionMenu(root, port, *ports, command=port_selected)
port_menu.grid(row=0, column=2)



def bass_slider_changed(value):
    send_to_serial(value, parameter.BASS)

def medium_slider_changed(value):
    send_to_serial(value, parameter.MEDIUM)

def light_status():
    global is_on
    send_to_serial(is_on, parameter.STATUS)


bass_slider = tk.Scale(root, from_=0, to=100, orient=tk.VERTICAL, command=bass_slider_changed)
bass_slider.grid(row=0, column=0)
tk.Label(root, text="Bass").grid(row=1, column=0)


medium_slider = tk.Scale(root, from_=0, to=100, orient=tk.VERTICAL, command=medium_slider_changed)
medium_slider.grid(row=0, column=1)
tk.Label(root, text="Medium").grid(row=1, column=1)




def on_off_switch_event():
    global is_on
    if is_on:
        is_on = False
        toggle_on__off_switch.config(text="OFF")
    else:
        is_on = True
        toggle_on__off_switch.config(text="ON")
    light_status()


toggle_on__off_switch = tk.Button(text="OFF", width=10, command=on_off_switch_event)
toggle_on__off_switch.grid(row=2, column=0, columnspan=2)







def send_to_serial(value, parameter):
    if parameter == parameter.BASS:
        print("Bass", value)
    elif parameter == parameter.MEDIUM:
        print("Medium", value)
    elif parameter == parameter.STATUS:
        print("Status", value)





root.mainloop()
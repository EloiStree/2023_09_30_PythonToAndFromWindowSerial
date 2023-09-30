import serial
import serial.tools.list_ports
import socket
import threading
import time


print(f"\nHello\n")

use_custom_filter=True

char_as_value_numeric= ['0','1','2','3','4','|','5','6','7','8','9']
char_as_value_alpha= ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','|','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']

def convert_char_value(char_value):

        print(f"\nLook for {char_value}")
        i=0.0;
        for  c in char_as_value_numeric:
            if char_value==c:
                #l=len(char_as_value_numeric)
                #rv=i/(len(char_as_value_numeric)-1)
                #print(f"\n{c}{char_value} -- {i}/{l-1}={rv}")
                return i/(len(char_as_value_numeric)-1)
            i+=1.0
            
        i=0.0;
        for  c in char_as_value_alpha:
            if char_value==c:
                return i/(len(char_as_value_alpha)-1)
            i+=1
    

def your_own_filtering(data_received):
    l = len(data_received)

    id=""
    c0=""
    c1=""
    if(l==1):
        c0=data_received[0]
        c1=""
    elif(l==2):
        c0=data_received[0]
        c0=data_received[1]
    elif(l==3):
        id= data_received[0]+data_received[1]
        c0=data_received[2]
        c1=""
    elif(l==4):
        id= data_received[0]+data_received[1]
        c0=data_received[2]
        c1=data_received[3]
    
    if l==1 or l==3:
        if c0=="A":
            data_received="bool:UnoDA:True"
        elif c0=="a":
            data_received="bool:UnoDA:False"
        elif c0=="B":
            data_received="bool:UnoDB:True"
        elif c0=="b":
            data_received="bool:UnoDB:False"
        elif c0=="C":
            data_received="bool:UnoDC:True"
        elif c0=="c":
            data_received="bool:UnoDC:False"
            
        elif c0=="D":
            data_received="bool:UnoDD:True"
        elif c0=="d":
            data_received="bool:UnoDD:False"
            
        elif c0=="E":
            data_received="bool:UnoDE:True"
        elif c0=="e":
            data_received="bool:UnoDE:False"
            
        elif c0=="F":
            data_received="bool:UnoDF:True"
        elif c0=="f":
            data_received="bool:UnoDF:False"
        elif c0=="G":
            data_received="bool:UnoDG:True"
        elif c0=="g":
            data_received="bool:UnoDG:False"
        elif c0=="H":
            data_received="bool:UnoDH:True"
        elif c0=="h":
            data_received="bool:UnoDH:False"
        elif c0=="I":
            data_received="bool:UnoDI:True"
        elif c0=="i":
            data_received="bool:UnoDI:False"
        elif c0=="J":
            data_received="bool:UnoDJ:True"
        elif c0=="j":
            data_received="bool:UnoDJ:False"
        elif c0=="K":
            data_received="bool:UnoDK:True"
        elif c0=="k":
            data_received="bool:UnoDK:False"
        elif c0=="L":
            data_received="bool:UnoDL:True"
        elif c0=="l":
            data_received="bool:UnoDL:False"
        elif c0=="M":
            data_received="bool:UnoDM:True"
        elif c0=="m":
            data_received="bool:UnoDM:False"
            
    if l==2 or l==4:
        name =""
        first_char = c0.lower()
        value= convert_char_value(c1)
        if first_char=='a':
            name="UnoAA"
        elif first_char=='b':
            name="UnoAB"
        elif first_char=='c':
            name="UnoAC"
        elif first_char=='d':
            name="UnoAD"
        elif first_char=='e':
            name="UnoAE"
        elif first_char=='f':
            name="UnoAF"
        elif first_char=='g':
            name="UnoAG"
        
        data_received=f"float:{name}:{value}"
    
        
    return data_received


use_manual_com_listener=True
com_ports_to_listen_manual_add = [
#"COM3",
    ]

device_description = [
    "USB Serial Device",
     "Arduino Mega 2560",
    ]


#parsing_tag_start="UnoRead|"
#parsing_tag_start="UR|"
parsing_tag_start=""
#parsing_tag_end="\n"
parsing_tag_end=""


# Define the UDP target information
UDP_TARGET_HOST = '25.38.254.222'  # Replace with the actual UDP target IP address or hostname
UDP_TARGET_PORT = 2506  # Replace with the actual UDP target port

# Function to send data to UDP target
def send_to_udp(data):
    if use_custom_filter:
        data= your_own_filtering(data)
    data= parsing_tag_start +data+parsing_tag_end
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.sendto(data.encode('utf-8'), (UDP_TARGET_HOST, UDP_TARGET_PORT))
    udp_socket.close()
    
    print(f"\nSent: {data}")

# Function to read and forward lines from a COM port to UDP
def read_and_forward(com_port):
    try:
        ser = serial.Serial(com_port, baudrate=9600, timeout=1)  # Adjust baudrate and timeout as needed
        while True:
            line = ser.readline().decode('utf-8').strip() # Read a line from the COM port
            if line:
                ## Dev note: I don't know why but if I don't use that, it will affect previous line instead of current...
                print(f"\nReceived: {line}")
                send_to_udp(line)
                
    except serial.SerialException as e:
        print(f"Error: {e}")
    finally:
        try:
            ser.close()
        except Exception as e:
            pass

# Get a list of available COM ports
available_com_ports = list(serial.tools.list_ports.comports())

if not available_com_ports:
    print("No COM ports found.")
else:
    # Initialize a separate list for the COM ports to listen to
    com_ports_to_listen = [
    ]


if use_manual_com_listener:
    for comNam in com_ports_to_listen_manual_add:
        com_ports_to_listen.append(comNam)
        



# Iterate through the list of COM ports and print their descriptions
for port_info in available_com_ports:
    port, desc, _ = port_info
    print(f"COM Port: {port}, Description: {desc}")
    for dev_desc in device_description:
        if dev_desc in desc.strip():
            com_ports_to_listen.append(port)

# Start a thread for each COM port to listen to
threads = []
for port in com_ports_to_listen:
    thread = threading.Thread(target=read_and_forward, args=(port,))
    threads.append(thread)
    thread.start()

# Wait for all threads to finish
for thread in threads:
    thread.join()

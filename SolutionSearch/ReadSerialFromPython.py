#pip install -U pypiwin32
import serial
import serial.tools.list_ports

'''
def scan():
    available = []
    for i in range(256):
        try:
            s = serial.Serial('COM'+str(i))
            available.append( (s.portstr))
            s.close()   # explicit close 'cause of delayed GC in java
            print ("%s" % (s))
        except serial.SerialException:
            pass

    for s in available:
        print ("%s" % (s))


if __name__=='__main__':
    print ("Found ports:")
    scan()
'''


# Get a list of available COM ports
com_ports = list(serial.tools.list_ports.comports())

if not com_ports:
    print("No COM ports found.")
else:
    # Iterate through the list of COM ports and print their descriptions
    for port_info in com_ports:
        port, desc, _ = port_info
        print(f"COM Port: {port}, Description: {desc}")
#!/usr/bin/env python3
import serial
import time

SERIAL_PORT = '/dev/ttyACM0'   # adjust if needed
BAUD = 115200
OUTPUT_FILE = 'bliss_output.txt'

def main():
    try:
        with serial.Serial(SERIAL_PORT, BAUD, timeout=1) as ser:
            with open(OUTPUT_FILE, 'wb') as f:
                print(f"Listening on {SERIAL_PORT}. Press Ctrl+C to stop.")
                while True:
                    line = ser.readline()
                    if line:
                        print(line.decode('utf-8', errors='replace').strip())
                        f.write(line)
    except KeyboardInterrupt:
        print("\nStopped.")
    except serial.SerialException as e:
        print(f"Error: {e}\nCheck port and Teensy connection.")

if __name__ == '__main__':
    main()

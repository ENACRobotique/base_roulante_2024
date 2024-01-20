#!/bin/env python3
import sys
sys.path.append('generated')
from serial import Serial
from serial.threaded import Protocol, ReaderThread
import time
from enum import Enum
import messages_pb2 as pb
import json
import socket
import struct

plotjuggler_udp = ("127.0.0.1", 9870)


class RxState(Enum):
    IDLE = 0
    HEAD_OK = 1
    GOT_LENGTH = 2
    

class Duckoder(Protocol):
    def __init__(self):
        Protocol.__init__(self)
        self.transport = None
        self._buffer = b'  '
        self._rx_state = RxState.IDLE
        self._msg_rcv = None
        self.so = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def connection_made(self, transport):
        self.transport = transport

    def data_received(self, data):
        for c in data:
            if self._decode(c.to_bytes(1, 'little')):
                m = pb.Message.FromString(self._msg_rcv)
                topic = m.WhichOneof('inner')
                jj = self.msg_to_json(m)
                self.so.sendto(jj.encode(), plotjuggler_udp)
                print(jj)

    def _decode(self, c):
        ret = False
        # décode la trame !
        return ret

    def msg_to_json(self, msg):
        msg_name = msg.WhichOneof('inner')
        inner = getattr(msg, msg_name)
        #popo = msg_name
        if msg_name == 'motors':
            msg_name = msg.motors.MotorDataType.Name(msg.motors.type)
        elif msg_name == 'pos':
            msg_name = msg.pos.PosObject.Name(msg.pos.obj)
        d = {msg_name: {}}
        for f in inner.DESCRIPTOR.fields:
            field_name = f.name
            d[msg_name][field_name] = getattr(inner, field_name)
        return json.dumps(d)


def send_message(ser: Serial, msg: pb.Message):
    # encode la trame !
    # Utilise struct, c'est pratique: https://docs.python.org/3/library/struct.html
    ser.write(b'')


def get_ex():
    m = pb.Message()
    m.bat.voltage = 23
    return m

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("missing port")
        exit(1)
    port = sys.argv[1]
    baudrate = 115200

    ser=Serial(port, baudrate)
    with ReaderThread(ser, Duckoder) as p:
        a = 3
        while True:
            msg = pb.Message()
            msg.bat.voltage = a
            a += 0.1
            send_message(ser, msg)
            time.sleep(0.1)


#!/bin/env python3
import sys
sys.path.append('generated')
from serial import Serial
from serial.threaded import Protocol, ReaderThread
import time
from enum import Enum
import messages_pb2 as llpb
import robot_state_pb2 as hgpb
import json
import socket
import struct
import math

import ecal.core.core as ecal_core
from ecal.core.publisher import ProtoPublisher
from ecal.core.subscriber import ProtoSubscriber

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
        self.send_plotjuggler = True
        if self.send_plotjuggler:
            self.so = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        ecal_core.initialize(sys.argv, "Bridge low level")
        self.odom_pos_pub = ProtoPublisher("odom_pos", hgpb.Position)
        self.carrot_pos_pub = ProtoPublisher("carrot_pos", hgpb.Position)
        self.target_pos_sub = ProtoSubscriber("set_position", hgpb.Position)
        self.target_pos_sub.set_callback(self.set_target)
        

    def connection_made(self, transport):
        self.transport = transport

    def data_received(self, data):
        for c in data:
            if self._decode(c.to_bytes(1, 'little')):
                m = llpb.Message.FromString(self._msg_rcv)
                if self.send_plotjuggler:
                    jj = self.msg_to_json(m)
                    self.so.sendto(jj.encode(), plotjuggler_udp)
                topic = m.WhichOneof('inner')
                if topic == "pos" and m.msg_type == llpb.Message.MsgType.STATUS:
                    hgm = hgpb.Position(x=m.pos.x,y=m.pos.y,theta=m.pos.theta)
                    if m.pos.obj == llpb.Pos.PosObject.POS_ROBOT_W:
                        self.odom_pos_pub.send(hgm)
                    elif m.pos.obj == llpb.Pos.PosObject.POS_CARROT_W:
                        self.carrot_pos_pub.send(hgm)
                

    def _decode(self, c):
        ret = False
        self._buffer += c
        match self._rx_state:
            case RxState.IDLE:
                if self._buffer[-1] == 0xFF and self._buffer[-2] == 0xFF:
                    self._buffer = self._buffer[-2:]
                    self._rx_state = RxState.HEAD_OK
            case RxState.HEAD_OK:
                self._expected_bytes = ord(c)+1
                self._rx_state = RxState.GOT_LENGTH
            case  RxState.GOT_LENGTH:
                self._expected_bytes -= 1
                if self._expected_bytes == 0:
                    chk = 0
                    for c in self._buffer[3:-1]:
                        chk ^= c
                    if chk == self._buffer[-1]:
                        ret = True
                        self._msg_rcv = self._buffer[3:-1]
                    else:
                        print("chk failed")
                    self._buffer = b'  '
                    self._rx_state = RxState.IDLE
        return ret

    def msg_to_json(self, msg):
        msg_name = msg.WhichOneof('inner')
        inner = getattr(msg, msg_name)
        if msg_name == 'motors':
            msg_name = msg.motors.MotorDataType.Name(msg.motors.type)
        elif msg_name == 'pos':
            msg_name = msg.pos.PosObject.Name(msg.pos.obj)
        d = {msg_name: {}}
        for f in inner.DESCRIPTOR.fields:
            field_name = f.name
            d[msg_name][field_name] = getattr(inner, field_name)
        return json.dumps(d)


    def send_message(self, msg: llpb.Message):
        payload = msg.SerializeToString()
        lenght = len(payload)
        crc = 0
        for b in payload:
            crc ^= b
        buffer = struct.pack('<BBB', 0xFF, 0xFF, lenght) + payload + struct.pack('<B', crc)
        # print([c for c in buffer])
        self.transport.write(buffer)
    
    def set_target(self, topic_name, hlm, time):
        print(hlm)
        llmsg = llpb.Message()
        llmsg.msg_type = llpb.Message.MsgType.COMMAND
        llmsg.pos.x = hlm.x
        llmsg.pos.y = hlm.y
        llmsg.pos.theta = hlm.theta
        llmsg.pos.obj = llpb.Pos.PosObject.POS_ROBOT_W
        print(llmsg)
        self.send_message(llmsg)



if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("missing port")
        exit(1)
    port = sys.argv[1]
    baudrate = 115200


    ser=Serial(port, baudrate)
    with ReaderThread(ser, Duckoder) as p:
        while True:
            # msg = llpb.Message()
            # msg.pos.x = 600
            # msg.pos.y = 2
            # msg.pos.theta = 0.1
            # msg.msg_type = llpb.Message.MsgType.COMMAND
            # p.send_message(msg)
            # print(msg)
            time.sleep(8)


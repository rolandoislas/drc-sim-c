import socket
import select

import os

PORT_WII_MSG = 50110
PORT_WII_VID = 50120
PORT_WII_AUD = 50121
PORT_WII_HID = 50122
PORT_WII_CMD = 50123
IP = "0.0.0.0"

sock_cmd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_cmd.bind((IP, PORT_WII_CMD))
sock_msg = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_msg.bind((IP, PORT_WII_MSG))
sock_hid = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_hid.bind((IP, PORT_WII_HID))
sock_vid = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_vid.bind((IP, PORT_WII_VID))
sock_aud = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_aud.bind((IP, PORT_WII_AUD))


def append_packet(file_name, sock):
    if not os.path.isdir("./dump"):
        os.mkdir("./dump")
    output = open("./dump/%s.bin" % file_name, "ab")
    output.write(sock.recv(2048) + b"||||\n")
    output.close()


def main():
    while True:
        rlist, wlist, xlist = select.select((sock_cmd, sock_msg, sock_hid, sock_vid, sock_aud), (), (), 1)
        if not rlist:
            continue
        for sock in rlist:
            if sock == sock_cmd:
                append_packet("cmd", sock_cmd)
            elif sock == sock_msg:
                append_packet("msg", sock_msg)
            elif sock == sock_hid:
                append_packet("hid", sock_hid)
            elif sock == sock_vid:
                append_packet("vid", sock_vid)
            elif sock == sock_aud:
                append_packet("aud", sock_aud)


if __name__ == '__main__':
    main()

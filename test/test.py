#!/usr/bin/env python
# -*- coding: gbk -*-


###########################################  
# File Name     : test.py
# Author        : liqibo(liqibo@baidu.com)
# Created Time  : 2017/8/10
# Brief         : test ad server
###########################################


__revision__ = '0.1'
import sys
import socket
import json


host = "127.0.0.1"
port = 60103


def run():

    data = "2\tflower"
    print data
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    s.send(bytes(data))
    data = s.recv(1024)
    print data

    return 0


def main():
    """
    statement
    """
    exit(run())


if __name__ == "__main__":
    main()






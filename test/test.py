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


def run():

    request_data = {
        "adnum": 2,
        "bidwords": ["鲜花"]
    }
    print str(request_data)

    return 0


def main():
    """
    statement
    """
    exit(run())


if __name__ == "__main__":
    main()






#!/usr/bin/env python  
# coding=UTF-8  
  
import ctypes

  
if __name__ == '__main__':  
		so = ctypes.cdll.LoadLibrary
		lib = so("./libHandleData.so")
		lib.GitData()               
from django.shortcuts import render
from django.http import HttpResponse,JsonResponse
from . import models
# Create your views here.

import ctypes

import random
import datetime
import json

def showlinediagram(request):
    article = models.stm32.objects.get(pk=1)
    return render(request, 'showlinediagram.html', {'article': article})

def add(request):
    #so = ctypes.cdll.LoadLibrary
    #lib = so("./libHandleData.so")
    #lib.GitData()
    stm32_data_cpu = []
    stm32_data_tem = []
    stm32_time_cpu = []
    stm32_time_tem = []
    stm32_cpu = {}
    stm32_tem = {}
    stm32_dirt = {}

    stm32_test = {}

    data = models.stm32.objects.all()
    lenght = data.count()
    stm32_ = data[lenght - 100:lenght]
    for stm32 in stm32_:
        stm32_data_cpu.append(stm32.time)
        stm32_time_cpu.append(stm32.data)
    stm32_dirt.update({
        'data': stm32_data_cpu,
        'time': stm32_time_cpu
    })
    print(stm32_dirt)
    return HttpResponse(json.dumps(stm32_dirt),content_type="application/json")

def test(request):
    return render(request, 'test.html')


def add1(request):
    stm32_data = []
    stm32_time = []
    stm32_dirt = {}
    for i in range(0, 10):
        stm32 = models.stm32.objects.get(pk=i)
        stm32_data.append(stm32.time)
        stm32_time.append(stm32.data)
    stm32_dirt.update({
        'data': stm32_data,
        'time': stm32_time
    })
    print(stm32_dirt)
    return HttpResponse(json.dumps(stm32_dirt),content_type="application/json")



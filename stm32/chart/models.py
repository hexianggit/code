from django.db import models

# Create your models here.
class cpu(models.Model):
    name = models.CharField(max_length=20,default='')
    data = models.CharField(max_length=20,default='')
    time = models.CharField(max_length=20,default='')
    state = models.CharField(max_length=20,default='')

class tem(models.Model):
    name = models.CharField(max_length=20,default='')
    data = models.CharField(max_length=20,default='')
    time = models.CharField(max_length=20,default='')
    state = models.CharField(max_length=20,default='')

class hum(models.Model):
    name = models.CharField(max_length=20,default='')
    data = models.CharField(max_length=20,default='')
    time = models.CharField(max_length=20,default='')
    state = models.CharField(max_length=20,default='')
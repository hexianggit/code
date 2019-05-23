from django.db import models

# Create your models here.
class stm32(models.Model):
    name = models.CharField(max_length=32,default='')
    time = models.CharField(max_length=32,default='')
    state = models.CharField(max_length=32,default='')
    data = models.CharField(max_length=32,default='')
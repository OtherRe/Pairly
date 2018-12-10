from django.shortcuts import render
from django.http import HttpResponse
import random
import django
import datetime

def charts(request):
    return render(request, 'charts/charts.html')

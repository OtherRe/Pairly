from django.shortcuts import render
from django.http import HttpResponse, JsonResponse
# from .forms import showDataFrom

def charts(request):
    return render(request, 'charts/charts.html')

def getData(request):
    data = {
        'labels': ["Red", "Blue", "Yellow", "Green", "Purple", "Orange"],
        'values': [12, 19, 3, 5, 2, 4],
    }
    return JsonResponse(data)
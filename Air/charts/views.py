from django.shortcuts import render
from django.http import HttpResponse, JsonResponse

# from .forms import showDataFrom

 
def charts(request):
    return render(request, 'charts/charts.html')

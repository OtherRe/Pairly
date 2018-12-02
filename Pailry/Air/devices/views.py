from django.shortcuts import render

# Create your views here.

def users_devices(request):
    return render(request, 'devices/device_list.html')
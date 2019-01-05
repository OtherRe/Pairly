from django.shortcuts import render, redirect
from .forms import NewDeviceForm
from .models import Device
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from django.http import HttpResponseNotFound
from ..Db import Db
import sys
sys.path.append('/home/jaro/Desktop/ZPRproject/PairlyDB/build')
import pypairly as db
# Create your views here.


        

@login_required(redirect_field_name='login')
def users_devices(request):
    devices = Db.mongo().getDevices(request.user.username)
    return render(request, 'devices/device_list.html', {'devices': devices})

def get_data_type(data_type):
    if data_type == 'PM2.5':
        return db.DataType.PM2_5
    elif data_type == 'PM10':
        return db.DataType.PM10
    elif data_type == 'CO':
        return db.DataType.CO

@login_required(redirect_field_name='login')
def new_device(request):
    if request.method == 'POST':
        form = NewDeviceForm(request.POST)
        if form.is_valid():
            device = db.Device(
                    '',
                    request.user.username,
                    form.cleaned_data.get('public_key'),
                    form.cleaned_data.get('name'),
                    form.cleaned_data.get('latitude'),
                    form.cleaned_data.get('longitude'),
                    get_data_type(form.cleaned_data.get('data_type'))
            )

            Db.mongo().addDevice(device)            
            return redirect('users_devices')

    else:
        devices = Db.mongo().getDevices(request.user.username)
        print(devices)
        if devices:
            initialCoords = {
                'latitude': devices[0].latitude, 
                'longitude': devices[0].longitude,
            }
        else:
            initialCoords = {
                'latitude': 52.237049, 
                'longitude': 21.017532,
            }
        form = NewDeviceForm(initial=initialCoords)
    return render(request, 'devices/new_device.html', {'form': form})

@login_required(redirect_field_name='login')
def device_info(request, name):
  #  public_key = request.GET.get('public_key')
    if not Device.objects.filter(name=name):
        return HttpResponseNotFound() 

    return render(request, 'devices/device_info.html', {'name':name})       

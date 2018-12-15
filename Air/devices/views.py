from django.shortcuts import render, redirect
from .forms import NewDeviceForm
from .models import Device
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from django.http import HttpResponseNotFound
import secrets
# Create your views here.

@login_required(redirect_field_name='login')
def users_devices(request):
    devices = request.user.devices.all()
    # if not devices:
    #     return render(request, 'devices/device_list.html')    
    return render(request, 'devices/device_list.html', {'devices': list(devices)})

@login_required(redirect_field_name='login')
def new_device(request):
    if request.method == 'POST':
        form = NewDeviceForm(request.POST)
        if form.is_valid():
            device = Device()
            device.name       = form.cleaned_data.get('name')
            device.public_key = form.cleaned_data.get('public_key')
            device.data_type  = form.cleaned_data.get('data_type')
            device.location   = form.cleaned_data.get('location')
            device.data_points= []
            device.user = request.user
            device.auth_token = secrets.token_urlsafe()
            device.save()
            return redirect('users_devices')
    else:
        form = NewDeviceForm()
    return render(request, 'devices/new_device.html', {'form': form})

@login_required(redirect_field_name='login')
def device_info(request, name):
  #  public_key = request.GET.get('public_key')
    if not Device.objects.filter(name=name):
        return HttpResponseNotFound() 

    return render(request, 'devices/device_info.html', {'name':name})       

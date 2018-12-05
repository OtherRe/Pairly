from django.shortcuts import render, redirect
from django.views.generic.edit import FormView, CreateView
from .forms import NewDeviceForm
from .models import Device, Data
from django.contrib.auth.decorators import login_required
from datetime import datetime
# Create your views here.

def users_devices(request):
    return render(request, 'devices/device_list.html')

@login_required(redirect_field_name='login')
def new_device(request):
    if request.method == 'POST':
        form = NewDeviceForm(request.POST)
        if form.is_valid():
            device = Device()
            device.public_key = form.cleaned_data.get('public_key')
            device.data_type  = form.cleaned_data.get('data_type')
            device.location   = form.cleaned_data.get('location')
            device.data_points= []
            device.user = request.user
            device.save()
            return redirect('users_devices')
    else:
        form = NewDeviceForm()
    return render(request, 'devices/new_device.html', {'form': form})

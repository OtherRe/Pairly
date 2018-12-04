from django.shortcuts import render, redirect
from django.views.generic.edit import FormView, CreateView
from .forms import NewDeviceForm
from .models import Device
from django.contrib.auth.decorators import login_required
# Create your views here.

def users_devices(request):
    return render(request, 'devices/device_list.html')

@login_required(redirect_field_name='login')
def new_device(request):
    if request.method == 'POST':
        form = NewDeviceForm(request.POST)
        if form.is_valid():

            return redirect('users_devices')
    else:
        form = NewDeviceForm()
    return render(request, 'devices/new_device.html', {'form': form})

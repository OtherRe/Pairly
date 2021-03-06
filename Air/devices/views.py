from django.shortcuts import render, redirect
from .forms import NewDeviceForm
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from django.core.exceptions import SuspiciousOperation
from django.http import HttpResponseNotFound, HttpResponseServerError, HttpResponseBadRequest
from ..Db import Db
import pypairly as db
# Create your views here.


@login_required(redirect_field_name='login')
def users_devices(request):
    try:
        devices = Db.mongo().getDevices(request.user.username)
    except RuntimeError:
        return HttpResponseServerError()
    return render(request, 'devices/device_list.html', {'devices': devices})


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
                Db.get_data_type(form.cleaned_data.get('data_type'))
            )

            Db.mongo().addDevice(device)
            return redirect('users_devices')

    else:
        try:
            devices = Db.mongo().getDevices(request.user.username)
        except RuntimeError:
            return HttpResponseServerError()

        if devices:
            initialCoords = {
                'latitude': devices[-1].latitude,
                'longitude': devices[-1].longitude,
            }
        else:
            initialCoords = {
                'latitude': 52.237049,
                'longitude': 21.017532,
            }
        form = NewDeviceForm(initial=initialCoords)
    return render(request, 'devices/new_device.html', {'form': form})


def device_info(request, id):
    try:
        device = Db.mongo().getDevice(id)
    except RuntimeError:
        return HttpResponseServerError()

    if device.name == '':
        return HttpResponseNotFound()

    return render(request, 'devices/device_info.html', {'device': device})


@login_required(redirect_field_name='login')
def remove_device(request):
    device_id = request.POST.dict().get('id')

    try:
        device = Db.mongo().getDevice(device_id)
    except RuntimeError:
        return HttpResponseServerError()

    if device.user != request.user.username:
        return HttpResponseBadRequest('Usernames did not match')

    try:
        Db.mongo().removeDevice(device_id)
    except RuntimeError:
        return HttpResponseServerError()

    return redirect('users_devices')

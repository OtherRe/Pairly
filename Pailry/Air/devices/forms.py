from django import forms
from django.forms import ModelForm, ValidationError
from .models import Device, Location
from .validators import validate_latitude, validate_longitute

class NewDeviceForm(ModelForm):
    class Meta:
        model = Device
        fields = ['public_key', 'data_type', 'location']

    
    def clean_location(self):
        data = self.cleaned_data['location']
        validate_latitude(data.lat)
        validate_longitute(data.lng)
        return data

    def clean_public_key(self):
        data = self.cleaned_data['public_key']
        
        if Device.objects.get(public_key=data) is not None:
            raise ValidationError('This public key already exists. Please check again.')
        
        return data


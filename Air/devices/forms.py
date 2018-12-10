from django import forms
from django.forms import ModelForm, ValidationError
from .models import Device, Location
from .validators import validate_latitude, validate_longitute
from Crypto.PublicKey import RSA

class NewDeviceForm(ModelForm):
    class Meta:
        model = Device
        fields = ['name', 'data_type', 'location', 'public_key']

    
    def clean_location(self):
        data = self.cleaned_data['location']
        validate_latitude(data.lat)
        validate_longitute(data.lng)
        return data

    def clean_public_key(self):
        data = self.cleaned_data['public_key']
        
        try:
            #RSA.importKey(data)
            a =1
        except:
            raise ValidationError("Wrong public key")

        if Device.objects.filter(public_key=data):
            raise ValidationError('This public key already exists. Please check again.')
        
        return data


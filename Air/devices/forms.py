from django import forms
from django.forms import ModelForm, ValidationError
from Crypto.PublicKey import RSA
from ..Db import Db

class NewDeviceForm(forms.Form):
    name = forms.CharField(max_length=20)
    data_type = forms.ChoiceField(choices=(
        ('CO', 'CO'), ('PM2.5', 'PM2.5'),('PM10', 'PM10'))) #what kind of air particles it measures

    public_key = forms.CharField(widget=forms.Textarea)
    latitude = forms.FloatField(min_value=-180, max_value=180, widget=forms.HiddenInput(), required=True, label='')
    longitude = forms.FloatField(min_value=-90, max_value=90, widget=forms.HiddenInput(), required=True, label='')

    def clean_public_key(self):
        public_key = self.cleaned_data['public_key']
        
        try:
            RSA.importKey(public_key)
            pass
        except:
            raise ValidationError("Wrong public key")

        devices = Db.mongo().getDevices()
        if list(filter(lambda device: device.pubKey == public_key, devices)):
            raise ValidationError('This public key already exists. Please check again.')
        
        return public_key

    
    def clean_name(self):
        name = self.cleaned_data['name']
        devices = Db.mongo().getDevices()#!!! users devices
        if list(filter(lambda device: device.name == name, devices)):
            raise ValidationError('You have a device with the same name. Please other name.')

        return name


    
    def __init__(self, *args, **kwargs):
        """
        overriding init to add bootstrap classes to visible
        elements
        """
        super(NewDeviceForm, self).__init__(*args, **kwargs)
        for visible in self.visible_fields():
            visible.field.widget.attrs['class'] = 'form-control col-sm-8 col-md-6 col-lg-4'


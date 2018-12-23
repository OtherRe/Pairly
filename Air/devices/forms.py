from django import forms
from django.forms import ModelForm, ValidationError
from .models import Device, Location
from .validators import validate_latitude, validate_longitute
from Crypto.PublicKey import RSA

class NewDeviceForm(forms.Form):
    # name = forms.CharField(max_length=25, attrs=form_attr)
    name = forms.CharField(max_length=20)
    data_type = forms.ChoiceField(choices=(
        ('CO', 'CO'), ('PM2.5', 'PM2.5'),('PM10', 'PM10'))) #what kind of air particles it measures

    public_key = forms.CharField(max_length=100, widget=forms.Textarea)
    latitude = forms.FloatField(min_value=-180, max_value=180, widget=forms.HiddenInput(), required=True, label='')
    longitude = forms.FloatField(min_value=-90, max_value=90, widget=forms.HiddenInput(), required=True, label='')

    
    # def clean_location(self):
    #     data = self.cleaned_data['location']
    #     validate_latitude(data.lat)
    #     validate_longitute(data.lng)
    #     return data

    def clean_public_key(self):
        data = self.cleaned_data['public_key']
        
        try:
            #RSA.importKey(data)
            pass
        except:
            raise ValidationError("Wrong public key")

        if Device.objects.filter(public_key=data):
            raise ValidationError('This public key already exists. Please check again.')
        
        return data

    
    def clean_name(self):
        name = self.cleaned_data['name']
        #check if users has this name in his inventory

        return data



    def __init__(self, *args, **kwargs):
        super(NewDeviceForm, self).__init__(*args, **kwargs)
        for visible in self.visible_fields():
            visible.field.widget.attrs['class'] = 'form-control col-sm-8 col-md-6 col-lg-4'


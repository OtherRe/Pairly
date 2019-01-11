from rest_framework import serializers
from .requests import MapDataRequest, GetDevicesInfoRequest, OneDeviceDataRequest
from rest_framework.exceptions import NotFound, AuthenticationFailed, ValidationError
import secrets
from time import time
import datetime
from ..Db import Db
import asymmetric_jwt_auth as jwt

class MapDataRequestSerializer(serializers.Serializer):
    longitude = serializers.FloatField(min_value = -180, max_value=180)
    latitude = serializers.FloatField(min_value = -90, max_value=90)
    data_type = serializers.ChoiceField(choices=(
        ('CO', 'CO'), ('PM2.5', 'PM2.5'),('PM10', 'PM10'))
    )
    radius = serializers.FloatField(min_value=0)
    after = serializers.DateField()
    before = serializers.DateField()

    def validate(self, data):
        """
        Make sure that after is before before
        """

        if data['after'] > data['before']:
            raise serializers.ValidationError("Begin date must be before end date")

        return data

    def create(self):
        return MapDataRequest(**self.validated_data)

class OneDeviceDataRequestSerializer(serializers.Serializer):
    device_id = serializers.CharField()
    after = serializers.DateField()

    def validate(self, data):
        """
        Make sure that after is before before
        """
        if data['after'] >= datetime.date.today(): 
            raise serializers.ValidationError("Begin date must be before end date")

        return data

    def create(self):
        return OneDeviceDataRequest(**self.validated_data)

class GetDevicesInfoRequestSerializer(serializers.Serializer):
    longitude = serializers.FloatField(min_value = -180, max_value=180)
    latitude = serializers.FloatField(min_value = -90, max_value=90)
    data_type = serializers.ChoiceField(choices=(
        ('CO', 'CO'), ('PM2.5', 'PM2.5'),('PM10', 'PM10'))
    )
    radius = serializers.FloatField(min_value=0)

    def create(self):
        return GetDevicesInfoRequest(**self.validated_data)

class PostDataSerializer(serializers.Serializer):
    authorization = serializers.CharField()
    device_id = serializers.CharField()
    value = serializers.FloatField(min_value = 0)

    def validate(self, data):
        try:
            device_id = jwt.token.get_claimed_username(data['authorization'])
        except:
            raise serializers.ValidationError('The token needs to be signed before sending')

        try:
            device = Db.mongo().getDevice(device_id)
        except:
           raise serializers.ValidationError('Invalid device id') 
        print(device)
        if device.name == '' or device_id != data['device_id']:
           raise serializers.ValidationError('Invalid device id') 

        try:
            validated = jwt.token.verify(data['authorization'], device.pubKey)
        except:
            raise serializers.ValidationError('Verification failed. There might be incorrect public key in database')
            
        if not validated:
            raise serializers.ValidationError('Public and private key didn\'t match')

        return data
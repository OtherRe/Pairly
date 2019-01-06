from rest_framework import serializers
from .requests import MapDataRequest, GetDevicesInfoRequest, OneDeviceDataRequest
from rest_framework.exceptions import NotFound, AuthenticationFailed, ValidationError
import secrets
from time import time

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
    before = serializers.DateField()

    def validate(self, data):
        """
        Make sure that after is before before
        """

        if data['after'] > data['before']:
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
    id = serializers.IntegerField()
    auth_token = serializers.CharField()
    public_key = serializers.CharField()
    value = serializers.FloatField(min_value=0)
    time = serializers.IntegerField()

    def validate_id(self, value):
        device = Device.objects.get(id=value)
        if not device:
            raise NotFound("Device with this id was not found")
        
        # return value
        return device

    def validate_time(self, value):
        current_time = time()
        if current_time < value or current_time > value + 3600:
            raise ValidationError("Date was to fat in the future of in the past")

        return value
    
    def validate(self, data):
        # device = Device.objects.get(id=data['id'])
        device = data['id']
        if not secrets.compare_digest(data['auth_token'], device.auth_token):
            raise AuthenticationFailed("Authentication via authentication token failed")

        if not secrets.compare_digest(data['public_key'], device.public_key):
            raise AuthenticationFailed("Public keys didn't match")

        return data
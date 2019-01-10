from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from .serializers import (MapDataRequestSerializer, 
        GetDevicesInfoRequestSerializer,
        OneDeviceDataRequestSerializer,
        PostDataSerializer)

from .throttling import DeviceRateThrottle
from django.shortcuts import render
import secrets
from django.utils.safestring import mark_safe
import json

# Create your views here.

class GetAreaData(APIView):
    """
    View to get a devices data to display a graph
    """
    def get(self, request, format=None):
        """
        Return formatted data as json
        Params:
        longitude - number representing lng of starting point of a search
        latitude  - number representing lat of starting point of a search
        data_type - 'CO', 'PM2.5' or 'PM10' as a data type of a seach
        radius - radius of a search
        after  - DateTime representing starting date of search
        before = DateTime representing ending   date of seach
        """
        data_request  = MapDataRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                error = {"message": "Query for area data was incorrect"}
                return Response(error, status=status.HTTP_400_BAD_REQUEST)

        data_response = data_request.create().make_query()
        return Response(data_response) 

class GetDeviceData(APIView):
    """
    View to get a device info(id and location) about particular device
    """
    def get(self, request, format=None):
        """
        Return formatted data as json
        """
        data_request  = OneDeviceDataRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                error = {"message": "Query for device data was incorrect"}
                return Response(error, status=status.HTTP_400_BAD_REQUEST)

        data_response = data_request.create().make_query()
        return Response(data_response)

class GetDevicesInfo(APIView):
    """
    View to get info(id and location) about devices
    on the map
    """
    def get(self, request, format=None):
        """
        Return formatted data as json
        """
        data_request = GetDevicesInfoRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                error = {"message": "Query for device data was incorrect"}
                return Response(error, status=status.HTTP_400_BAD_REQUEST)

        data_response = data_request.create().make_query()
        return Response(data_response)

class PostData(APIView):
    """
    View to manage posting data by devices with authentication
    """
    # throttle_classes = (DeviceRateThrottle,)

    def post(self, request):
        data = PostDataSerializer(data=request.POST.dict())
        data.is_valid()
        data = data.validated_data

        # db.addData(data['id'].id, (data['time'], data['value']))
        
        return Response({}, status=status.HTTP_201_CREATED)
        
def index(request):
        return render(request, 'data/test.html')

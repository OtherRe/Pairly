from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from .serializers import (MapDataRequestSerializer, 
        GetDevicesInfoRequestSerializer,
        OneDeviceDataRequestSerializer,
        PostDataSerializer)

from ..Db import Db
from django.shortcuts import render
import time


# Create your views here.

class GetAreaData(APIView):
    """
    View to get a devices data to display a graph. Given parameters listed
    below returns two lists value_axis and time_axis with calculated mean
    data from devices
    """
    def get(self, request, format=None):
        """
        Return formatted data as json
        :param longitude: number representing lng of starting point of a search
        :param latitude: number representing lat of starting point of a search
        :param data_type: 'CO', 'PM2.5' or 'PM10' as a data type of a seach
        :param radius: radius of a search
        :param after: DateTime representing starting date of search
        :param before: DateTime representing ending   date of seach
        """
        data_request  = MapDataRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                return Response(data_request.errors, status=status.HTTP_400_BAD_REQUEST)

        data_response = data_request.create().make_query()
        return Response(data_response, status=status.HTTP_200_OK) 

class GetDeviceData(APIView):
    """
    View to get a device info(id and location) about particular device.
    Given parameters listed
    below returns two lists value_axis and time_axis with calculated mean
    data from device
    """
    def get(self, request, format=None):
        """
        Return formatted data as json, as two lists with value_axis and time_axis
        :param device_id: id string of a device
        :param after: DateTime representing starting date of search
        :param before: DateTime representing ending   date of seach
        """
        data_request  = OneDeviceDataRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                return Response(data_request.errors, status=status.HTTP_400_BAD_REQUEST)

        data_response = data_request.create().make_query()
        return Response(data_response, status=status.HTTP_200_OK)

class GetDevicesInfo(APIView):
    """
    View to get info(id and location) about devices
    on the map.
    """
    def get(self, request, format=None):
        """
        Return formatted data as json, as two list with objects containing name location
        and id of a device
        :param device_id: id string of a device
        :param after: DateTime representing starting date of search
        :param before: DateTime representing ending   date of seach
        """
        data_request = GetDevicesInfoRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                return Response(data_request.errors, status=status.HTTP_400_BAD_REQUEST)

        data_response = data_request.create().make_query()
        return Response(data_response, status=status.HTTP_200_OK)

class PostData(APIView):
    """
    View to manage posting data by devices with authentication
    In order to post data use our script called send_data_script.py
    """

    def post(self, request):
        data_request = PostDataSerializer(data=request.POST.dict())
        if not data_request.is_valid():
                return Response(data_request.errors, status=status.HTTP_400_BAD_REQUEST)

        data = data_request.validated_data
        Db.mongo().addData(data['device_id'], Db.createData(data['value'], int(time.time())))
        
        return Response({}, status=status.HTTP_201_CREATED)

from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from .serializers import (DataRequestSerializer, 
        GetDevicesInfoRequestSerializer,
        OneDeviceDataRequestSerializer,
        PostDataSerializer)

from .data_models import DataResponse
from .throttling import DeviceRateThrottle
import secrets

# Create your views here.

class GetAreaData(APIView):
    """
    View to get a devices data to display a graph
    """
    def get(self, request, format=None):
        """
        Return formatted data as json
        """
        data_request  = DataRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                error = {"message": "Query for area data was incorrect"}
                return Response(error, status=status.HTTP_400_BAD_REQUEST)

        data_response = DataResponse(data_request=data_request.create()).getData()
        return Response(data_response.data)

class GetDeviceData(APIView):
    """
    View to get a devices data to display a graph
    """
    def get(self, request, format=None):
        """
        Return formatted data as json
        """
        data_request  = OneDeviceDataRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                error = {"message": "Query for device data was incorrect"}
                return Response(error, status=status.HTTP_400_BAD_REQUEST)

        data_response = DataResponse(data_request=data_request.create()).getData()
        return Response(data_response.data)

class GetDevicesInfo(APIView):
    """
    View to get a devices data to display a graph
    """
    def get(self, request, format=None):
        """
        Return formatted data as json
        """
        data_request  = GetDevicesInfoRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                error = {"message": "Query for device data was incorrect"}
                return Response(error, status=status.HTTP_400_BAD_REQUEST)

        # data_response = DataResponse(data_request=data_request.create()).getData()
        # return Response(data_response.data)
        return Response({'devices': [
                {'lat' : 52.237049, 'lng':21.017532, 'id':1},
                {'lat' : 52.247049, 'lng':21.017532, 'id':2},
                {'lat' : 52.257049, 'lng':21.017532, 'id':3},
        ]})

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
        
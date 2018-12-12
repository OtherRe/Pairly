from django.shortcuts import render
from django.http import HttpResponse, JsonResponse
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import serializers
from rest_framework import status

# from .forms import showDataFrom

 
def charts(request):
    return render(request, 'charts/charts.html')

# class DataResponse():
#     def __init__(self, *args, **kwargs):
#         self.time_axis = kwargs['time_axis'] if 'time_axis' in kwargs else []
#         self.time_axis = kwargs['value_axis'] if 'value_axis' in kwargs else []
#         self.value_axis = value_axis


class DataRequest():
    def __init__(self, lng, lat, data_type, radius):
        self.lng = lng
        self.lat = lat
        self.data_type = data_type
        self.radius = radius

class DataRequestSerializer(serializers.Serializer):
    longitude = serializers.FloatField(min_value = -180, max_value=180)
    latitude = serializers.FloatField(min_value = -90, max_value=90)
    data_type = serializers.ChoiceField(choices=(
        ('CO', 'CO'), ('PM2.5', 'PM2.5'),('PM10', 'PM10'))
    )
    radius = serializers.FloatField(min_value=0)



class GetData(APIView):
    """
    View to get a devices data to display a graph
    """
    def get(self, request, format=None):
        """
        Return formatted data as json
        """
        data_request  = DataRequestSerializer(data=request.GET.dict())
        if not data_request.is_valid():
                error = {"message": "Query for device data was incorrect"}
                return Response(error, ststus=status.HTTP_400_BAD_REQUEST)

        data_response = [["Red", "Blue", "Yellow", "Green", "Purple", "Orange"],
                                 [12, 16, 3, 5, 2, 4]]

        # data_response = DataResponse(data_request)
        data = {
            'labels': data_response[0],
            'values': data_response[1],
        }
        return Response(data)
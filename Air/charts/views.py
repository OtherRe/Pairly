from django.shortcuts import render
from django.http import HttpResponse, JsonResponse
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import serializers

# from .forms import showDataFrom

 
def charts(request):
    return render(request, 'charts/charts.html')

class GetDataSerializer(serializers.Serializer):
    pass

class GetData(APIView):
    """
    View to get a devices data to display a graph
    """
    def get(self, request, format=None):
        """
        Return formatted data as json
        """
        data = {
            'labels': ["Red", "Blue", "Yellow", "Green", "Purple", "Orange"],
            'values': [12, request.GET.get('radius'), 3, 5, 2, 4],
        }
        return Response(data)
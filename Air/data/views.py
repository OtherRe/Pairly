from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from .serializers import DataRequestSerializer
from .data_models import DataRequest, DataResponse

# Create your views here.

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
                return Response(error, status=status.HTTP_400_BAD_REQUEST)

        data_response = DataResponse(data_request=data_request.create()).get()
        data = {
            'labels': data_response.time_axis,
            'values': data_response.value_axis,
        } 
        return Response(data)
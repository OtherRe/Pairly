from django.urls import path

from . import consumers

websocket_urlpatterns = [
    path('data/postData/<id>', consumers.PostDataConsumer),
]
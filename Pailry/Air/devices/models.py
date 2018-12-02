from django.db import models

# Create your models here.
from djongo import models
from django.contrib.auth.models import User

# Create your models here.

class Location(models.Model):
    lng = models.FloatField('Longitude', 'lng')
    lat = models.FloatField('Latitude', 'lat')

    def __str__(self):
        return "Location: {} longitute, {} latitue".format(self.lng, self.lat)

    class Meta:
        abstract = True
    
class Data(models.Model):
    send_date = models.DateTimeField()
    value = models.FloatField('Measured value', 'value')

    def __str__(self):
        return "Data from {} with value : {}".format(self.send_date, self.value)
    class Meta:
        abstract = True

class Device(models.Model):
    public_key = models.CharField(max_length=30)
    data_type = models.CharField(max_length=5) #what kind of air particles it measures
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='devices') # 1 to many
    location = models.EmbeddedModelField(
        model_container=Location,
    )
    data_points = models.ArrayModelField(
        model_container=Data,
    )
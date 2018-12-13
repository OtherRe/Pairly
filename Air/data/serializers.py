from rest_framework import serializers
from .data_models import DataRequest

class DataRequestSerializer(serializers.Serializer):
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
        return DataRequest(**self.validated_data)
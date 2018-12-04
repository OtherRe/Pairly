from django.core.validators import MinValueValidator, MaxValueValidator
from django.forms import FloatField
from djongo import models

# class LongitudeField(models.FloatField):
#     def __init__(self, *args, **kwargs):
#         super().__init__(*args, **kwargs)

#     default_validators=[ MinValueValidator(-90, ERROR_MSG.format('Longitude', -90, 90)),
#                         MaxValueValidator(90, ERROR_MSG.format('Longitude', -90, 90)),]

# class LatitudeField(models.FloatField):
#     def __init__(self, *args, **kwargs):
#         super().__init__(*args, **kwargs)

#     default_validators=[MinValueValidator(-180, ERROR_MSG.format('Longitude', -180, 180)),
#                         MaxValueValidator(180, ERROR_MSG.format('Longitude', -180, 180)),]
from django.core.exceptions import ValidationError

def validate_latitude(value):
    if value < -90 or value > 90:
        raise ValidationError(
            ('Latitude should be between -90 and 90 degrees, not: %(value)s'),
            params={'value': value}
        )

def validate_longitute(value):
    if value < -180 or value > 180:
        raise ValidationError(
            ('Longitude should be between -180 and 180 degrees, not: %(value)s'),
            params={'value':value}
    )
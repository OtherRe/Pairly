from rest_framework.throttling import UserRateThrottle, AnonRateThrottle
class DeviceRateThrottle(AnonRateThrottle):
    scope = 'device'

import time
from ..Db import Db
from ..devices.views import get_data_type
from datetime import datetime
from itertools import tee

def pairwise(iterable):
    "s -> (s0,s1), (s1,s2), (s2, s3), ..."
    a, b = tee(iterable)
    next(b, None)
    return zip(a, b)

# def make_time_axis(value_time_pairs):
#     axis = str(datetime.fromtimestamp(value_time_pairs[0].second))
#         for (index, (prev, curr)) in enumerate(pairwise(value_time_pairs)):
#             prev = str(datetime.fromtimestamp(prev.second)).split(' ')
#             curr = str(datetime.fromtimestamp(curr.second)).split(' ')
#             prev_date, prev_time = (prev[0], prev[1])
#             curr_date, curr_time = (curr[0], curr[1])


#             axis = curr_date + ' ' + 

class MapDataRequest():
    """
    Model that represents a request for a specific data.
    Given location data type radius and time range returns
    data collected by website
    """

    def __init__(self, longitude, latitude, data_type, radius, after, before):
        self.longitude = longitude
        self.latitude = latitude
        self.data_type = get_data_type(data_type) #need to convert to c++ enum
        self.radius = radius
        # self.after = int(time.mktime(after.timetuple()))
        self.after = 0
        self.before = 159000
        # self.before = int(time.mktime(before.timetuple()))

    def to_list(self):
        interval = (self.before - self.after)//10 //3600 + 1
        return [self.latitude, self.longitude, self.radius, self.after,
                self.before, interval, self.data_type]

    def make_query(self):
        value_time_pair_data = Db.mongo().getAreaData(*self.to_list())
        value_axis = [pair.first for pair in value_time_pair_data]
        time_axis  = [str(datetime.fromtimestamp(pair.second)) for pair in value_time_pair_data]
        return {'time_axis': time_axis, 'value_axis':value_axis}


class OneDeviceDataRequest():
    """
    Model that represents a request for a specific data.
    This one fetches collected data from a single device
    """

    def __init__(self, device_id, after, before):
        self.device_id = device_id
        self.after = int(time.mktime(after.timetuple()))
        self.before = int(time.mktime(before.timetuple()))

    def to_list(self):
        return [self.device_id, (self.before - self.after)//10 // 3600, self.after,
                self.before]

    def make_query(self):
        value_time_pair_data = Db.mongo().getDeviceData(*self.to_list())
        print(list(value_time_pair_data))
        value_axis = [pair.first for pair in value_time_pair_data]
        time_axis  = [str(datetime.fromtimestamp(pair.second)) for pair in value_time_pair_data]
        return {'time_axis': time_axis, 'value_axis':value_axis}


class GetDevicesInfoRequest():
    """
    Model that represents a request for a specific data.
    Returns devices info(name and location) about devices
    from given area.
    """

    def __init__(self, longitude, latitude, radius, data_type):
        self.longitude = longitude
        self.latitude = latitude
        self.radius = radius
        self.data_type = get_data_type(data_type)

    def to_list(self):
        return [self.latitude, self.longitude, self.radius,
                self.data_type]

    def make_query(self):
        devices = Db.mongo().getDevices(*self.to_list())
        devices = [{'name': d.name, 'lat': d.latitude, 'lng':d.longitude, 'id':d.id} for d in devices]
        print(*self.to_list())
        print(devices)
        return {'devices': list(devices)}

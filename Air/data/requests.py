import time


class DataRequest():
    """
    Model that represents a request for a specific data.
    Everything database need to know is coordinates, data_type
    and radius, before and after
    """

    def __init__(self, longitude, latitude, data_type, radius, after, before):
        self.longitude = longitude
        self.latitude = latitude
        self.data_type = data_type
        self.radius = radius
        self.after = time.mktime(after.timetuple())
        self.before = time.mktime(before.timetuple())

    def to_list(self):
        return [self.latitude, self.longitude, self.radius, self.after,
                self.before, (self.before - self.after)/10, self.data_type]

    def make_query(self, db):
        pass
        # time_value_pair_data = self.db.getAreaData(*self.to_list())
        # time_axis  = [pair[0] for pair in time_value_pair_data]
        # value_axis = [pair[1] for pair in time_value_pair_data] #THIS IS CORRECT
        # return {'time_axis': time_axis, 'value_axis':value_axis}


class OneDeviceDataRequest():
    """
    Model that represents a request for a specific data.
    Everything database need to know is coordinates, data_type
    and radius, before and after
    """

    def __init__(self, device_id, after, before):
        self.device_id = device_id
        self.after = time.mktime(after.timetuple())
        self.before = time.mktime(before.timetuple())

    def to_list(self):
        return [self.device_id, (self.before - self.after)/10, self.after,
                self.before]

    def make_query(self, db):
        pass
        # time_value_pair_data = self.db.getDeviceData(*self.to_list())
        # time_axis  = [pair[0] for pair in time_value_pair_data]
        # value_axis = [pair[1] for pair in time_value_pair_data] #THIS IS CORRECT
        # return {'time_axis': time_axis, 'value_axis':value_axis}


class GetDevicesInfoRequest():
    """
    Model that represents a request for a specific data.
    Everything database need to know is coordinates, data_type
    and radius, before and after
    """

    def __init__(self, longitude, latitude, radius, data_type):
        self.longitude = longitude
        self.latitude = latitude
        self.radius = radius
        self.data_type = data_type

    def to_list(self):
        return [self.longitude, self.latitude, self.radius,
                self.data_type]

    def make_query(self, db):
        pass
        # devices = self.db.getDevices(*self.to_list())
        # return {'devices': devices}

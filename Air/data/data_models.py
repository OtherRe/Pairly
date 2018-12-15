from math import inf
import time
import math
from datetime import date
class Device():
    def __init__(self, id, name, data_type, username, lng, lat):
        self.name = name
        self.data_type =data_type
        self.username = username
        self.lng = lng
        self.lat = lat
        self.id = id
        self.data = []
    
    def __str__(self):
        return 'Device {}'.format(self.name)

class DataResponse():
    """
    Class that takes DataRequest object, connects to a database and
    returns formatted data for charts as lists of
    x-axis values -> time_axis
    y-axis values -> value_axis
    
    """
    def __init__(self, *args, **kwargs):
        """
        Constructor that takes data_request and saves it for later use.
        User may also pass pre_prepared data as time_axis or value_axis.
        Acts as a lazy object. It means that it only once accesses the 
        database per data_request via method get(). If user wants to search\
        the database again, one should use reset() method

        :param time_axis: list of time stamps
        :param value_axis: list of value stamps for every time stamp
        :param data_request: DataRequest object
        :param db: Database object we want to use
        """
        self.data_request = kwargs['data_request'] if 'data_request' in kwargs else None
        self.db           = kwargs['db'] if 'db' in kwargs else None #!!!!!change later to cpp lib 
        self.data         = {}
        self.requested_before = False

    def getData(self):
        """
        Connects to a database, formats the data and stores it in
        value_axis and time_axis
        :return: itself
        """
        if self.requested_before:
            return self
        
        self.data['time_axis']  = ["Reds", "Blue", "Yellow", "Green", "Purple", "Orange", 'rafal']#TODELETE
        self.data['value_axis'] = [12, 11, 13, 5, 2, 4, 18]                                       #TODELETE

        if self.data_request is None:
            self.data = {}
            return self

        # self.data = self.data_request.make_query(self.db)

        return self

    def reset(self):
        """Resets the data response in order to look up database again
        """
        self.requested_before = False
        self.time_axis = []
        self.value_axis = []

    # def force_get(self):
    #     self.reset()
    #     self.get()

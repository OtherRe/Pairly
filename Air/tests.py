from django.test import TestCase
from .mocks import DbMock
from .data.data_models import Device, DataRequest, DataResponse
from datetime import date
import time

class DataResponseTestCase(TestCase):
    def setUp(self):
        self.db = DbMock()
        device = Device(1, 'device1', 'CO', 'john', 0, 0)
        self.db.addDevice(device)
        device = Device(2, 'device2', 'CO', 'john', 3, 0)
        self.db.addDevice(device)

        for i in range(7):
            self.db.addData(1, (i+2, i + 10))

        for i in range(7):
            self.db.addData(2, (i+3, i + 20))

    def test_GivenOneDevice_WhenRequestingWholeTimePeriod_ReturnsCorrectData(self):
        request = DataRequest(0,0, 'CO', 2, date.fromtimestamp(0), date.fromtimestamp(150000))
        data = DataResponse(data_request=request, db=self.db).get()

        self.assertEqual(data.time_axis, [2,3,4,5,6,7,8], "Time axis was not correct")
        self.assertEqual(data.value_axis, [10,11,12,13,14,15,16], "Value axis was not correct")

    def test_GivenTwoDevices_WhenRequestingWholeTimePeriod_ReturnsCorrectData(self):
        request = DataRequest(0,0, 'CO', 4, date.fromtimestamp(0), date.fromtimestamp(150000))
        data = DataResponse(data_request=request, db=self.db).get()

        self.assertEqual(data.time_axis, [2,3,4,5,6,7,8,3,4,5,6,7,8,9], "Time axis was not correct")
        self.assertEqual(data.value_axis, [10,11,12,13,14,15,16,20,21,22,23,24,25,26], "Value axis was not correct")
            
# Create your tests here.



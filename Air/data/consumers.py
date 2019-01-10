from channels.generic.websocket import WebsocketConsumer
from ..Db import Db
import json
import random

class PostDataConsumer(WebsocketConsumer):
    def connect(self):
        self.accept()

    def disconnect(self, close_code):
        pass

    def receive(self, text_data):
        #użyj np. Db.mongo().getDevices(), albo Db.mongo().addData(Db().createData(11.0, 2139218))
        text_data_json = json.loads(text_data)
        message = text_data_json['message']
        self.send(text_data=json.dumps({
            'message': message
        }))
import sys, os
sys.path.append(sys.path.append(os.path.join(os.path.dirname(__file__), "../PairlyDB/build")))
import pypairly as db

class Db:
    _mongo = db.PairlyDB.instance()

    @staticmethod
    def mongo():
        if True:
          try:          
            db.PairlyDB.connect("mongodb://127.0.0.1:27017", "pairlyDB")
          except:
            pass

          return Db._mongo


    @staticmethod
    def get_data_type(data_type):
      if data_type == 'PM2.5':
          return db.DataType.PM2_5
      elif data_type == 'PM10':
          return db.DataType.PM10
      elif data_type == 'CO':
          return db.DataType.CO

    @staticmethod
    def createData(*args):
        return db.Data(*args)
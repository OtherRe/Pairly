import sys, os
sys.path.append(sys.path.append(os.path.join(os.path.dirname(__file__), "../PairlyDB/build")))
import pypairly as db

class Db:
    """
    Proxy class for the mongo database. This class assures that
    database is connected and if not connects it.
    """
    _mongo = db.PairlyDB.instance()

    @staticmethod
    def mongo():
        """
        Get an instance of a mongo database
        """
        if True:
          try:          
            db.PairlyDB.connect("mongodb://127.0.0.1:27017", "pairlyDB")
          except:
            pass

          return Db._mongo


    @staticmethod
    def get_data_type(data_type):
      """
      Converts a string into an c++ enum 
      """
      if data_type == 'PM2.5':
          return db.DataType.PM2_5
      elif data_type == 'PM10':
          return db.DataType.PM10
      elif data_type == 'CO':
          return db.DataType.CO

    @staticmethod
    def createData(*args):
        """
        Creates paitly.Data objects from args
        """
        return db.Data(*args)
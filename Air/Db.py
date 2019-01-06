import sys
sys.path.append('/home/jaro/Desktop/ZPRproject/PairlyDB/build')
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
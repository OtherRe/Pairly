import requests
import asymmetric_jwt_auth as jwt
import sys


if __name__ == '__main__':

  if len(sys.argv) != 2:
    print('Wrong number of arguments')
    sys.exit(1)

  device_id = '5c37b29fb5862265a268aad2'                          #paste id of your device here
  private_key = jwt.load_private_key(key_file='test-private.pem') #paste a path to private key of your device
  
  auth = jwt.token.sign(username=device_id, private_key=private_key).decode('utf-8')

  r = requests.post('http://127.0.0.1:8000/data/PostData/', data = {
      'authorization': auth,
      'value': sys.argv[1],
      'device_id': device_id,
  })
  print(r.status_code)

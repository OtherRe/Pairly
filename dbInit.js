db = connect("localhost:27017/pairlyDB");

devices = db.devices
data = db.data

devices.createIndex( { location: "2dsphere" } )
data.createIndex( { device: "hashed" } )


var currentMapBounds;
var circle;
var defaultLng = document.getElementById('id_longitude').value
var defaultLat = document.getElementById('id_latitude').value
var defaultRadius = document.getElementById('id_radius').value

var mymap = mapSetup(defaultLng, defaultLat);

putMarkerAndResfreshDevices({
    'latlng': {
        'lng': defaultLng,
        'lat': defaultLat
    }
})

mymap.on('click', putMarkerAndResfreshDevices);

function putMarkerAndResfreshDevices(e) {
    currentPositionMarker.setLatLng(e.latlng)

    const lng = e.latlng.lng
    const lat = e.latlng.lat
    const radius = document.getElementById('id_radius').value

    //modyfing the form
    document.getElementById('id_longitude').value = lng
    document.getElementById('id_latitude').value = lat

    circle = putCircle(mymap, circle, lng, lat, radius)

    if (!currentMapBounds || !currentMapBounds.contains(e.latlng)) {
        currentMapBounds = mymap.getBounds()
        showDevices(mymap, '/data/GetDevicesInfo/', lng, lat, (currentMapBounds.getNorth() - currentMapBounds.getSouth()) / 2 * 1111)
    }
}

document.getElementById('id_radius').onchange = () => {
    const radius = document.getElementById('id_radius').value
    const lng = document.getElementById('id_longitude').value
    const lat = document.getElementById('id_latitude').value

    circle = putCircle(mymap, circle, lng, lat, radius)
}

document.getElementById('id_data_type').onchange = () => {
    console.log("dupa")
    const lng = document.getElementById('id_longitude').value
    const lat = document.getElementById('id_latitude').value
    showDevices(mymap, '/data/GetDevicesInfo/', lng, lat, (currentMapBounds.getNorth() - currentMapBounds.getSouth()) / 2 * 111)

}
const defaultLng = document.getElementById('id_longitude').value
const defaultLat = document.getElementById('id_latitude').value

var mymap = mapSetup(defaultLng, defaultLat);
console.log("devicemap")

putMarker({
    'latlng': {
        'lng': defaultLng,
        'lat': defaultLat
    }
})

mymap.on('click', putMarker);

function putMarker(e) {
    currentPositionMarker.setLatLng(e.latlng)

    const lng = e.latlng.lng
    const lat = e.latlng.lat

    //modyfing the form
    document.getElementById('id_longitude').value = lng
    document.getElementById('id_latitude').value = lat
}

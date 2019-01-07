var deviceMarkers = new Array();
var greenIcon = new L.Icon({
    iconUrl: 'https://cdn.rawgit.com/pointhi/leaflet-color-markers/master/img/marker-icon-2x-green.png',
    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',
    iconSize: [25, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
    shadowSize: [41, 41]
});
var currentPositionMarker;


function mapSetup(defaultLng, defaultLat) {
    var map = L.map('mapid').setView([defaultLat, defaultLng], 13);

    L.tileLayer('https://api.tiles.mapbox.com/v4/{id}/{z}/{x}/{y}.png?access_token=pk.eyJ1IjoibWFwYm94IiwiYSI6ImNpejY4NXVycTA2emYycXBndHRqcmZ3N3gifQ.rJcFIG214AriISLbB6B5aw', {
        maxZoom: 18,
        attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors, ' +
            '<a href="https://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, ' +
            'Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
        id: 'mapbox.streets'
    }).addTo(map);


    currentPositionMarker = L.marker([defaultLat, defaultLng], {
        'icon': greenIcon
    })
    currentPositionMarker.addTo(map);

    return map;
}


/**
 * 
 * @param {*} map 
 * @param {*} url 
 * @param {*} lng 
 * @param {*} lat 
 * @param {*} radius 
 * Fetches the data from database and puts makers for every
 * fetched deviece on map
 */
function showDevices(map, url, lng, lat, radius) {
    const data_type = document.getElementById('id_data_type').value

    const httpClient = new HttpClient()
    httpClient.getJSONWithArgs(url, [
        ['longitude', lng],
        ['latitude', lat],
        ['data_type', data_type],
        ['radius', radius]
    ], data => {
        markDevices(map, data.devices)
    })
}
/**
 * 
 * @param {list of info about devices - lng, lat, id} devices 
 * Puts a mark on the map for every device in devices
 */
function markDevices(map, devices) {
    deviceMarkers.forEach((m) => map.removeLayer(m))
    deviceMarkers = new Array()

    for (const info of devices) {
        var m = L.marker([info.lat, info.lng])
        m.addTo(map)
            .bindPopup(`Device with name: <a href="/devices/device_info/${info.id}">${info.name}</a>`)
        deviceMarkers.push(m)
    }
}

function putCircle(map, circle, lng, lat, radius) {
    if (circle)
        map.removeLayer(circle)

    circle = L.circle([lat, lng], radius * 1000, {
        color: 'red',
        fillColor: '#f03',
        fillOpacity: 0.5
    })

    circle.addTo(map)

    return circle;
}
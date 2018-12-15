var mymap;

// L.marker([51.5, -0.09]).addTo(mymap)
//     .bindPopup("<b>Hello world!</b><br />I am a popup.").openPopup();



// L.circle([51.508, -0.11], 500, {
//     color: 'red',
//     fillColor: '#f03',
//     fillOpacity: 0.5
// }).addTo(mymap).bindPopup("I am a circle.");

// L.polygon([
//     [51.509, -0.08],
//     [51.503, -0.06],
//     [51.51, -0.047]
// ]).addTo(mymap).bindPopup("I am a polygon.");


// var popup = L.popup();

function mapSetup(url) {
    var defaultLng = document.getElementById('id_longitude').value
    var defaultLat = document.getElementById('id_latitude').value

    mymap = L.map('mapid').setView([defaultLat, defaultLng], 13);

    L.tileLayer('https://api.tiles.mapbox.com/v4/{id}/{z}/{x}/{y}.png?access_token=pk.eyJ1IjoibWFwYm94IiwiYSI6ImNpejY4NXVycTA2emYycXBndHRqcmZ3N3gifQ.rJcFIG214AriISLbB6B5aw', {
        maxZoom: 18,
        attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/">OpenStreetMap</a> contributors, ' +
            '<a href="https://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, ' +
            'Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
        id: 'mapbox.streets'
    }).addTo(mymap);
    showDevices(url);
}

function showDevices(url) {
    const lng = document.getElementById('id_longitude').value
    const lat = document.getElementById('id_latitude').value
    const data_type = document.getElementById('id_data_type').value
    const radius = document.getElementById('id_radius').value

    const httpClient = new HttpClient()
    httpClient.getJSONWithArgs(url, [
        ['longitude', lng],
        ['latitude', lat],
        ['data_type', data_type],
        ['radius', radius]
    ], data => {
        markDevices(data.devices)
    })
}
/**
 * 
 * @param {list of info about devices - lng, lat, id} devices 
 * Puts a mark on the map for every device in devices
 */
function markDevices(devices) {
    for (const info of devices) {
        L.marker([info.lat, info.lng]).addTo(mymap)
            .bindPopup(`Device with id ${info.id}`)
    }
}

function onMapClick(e) {
    popup
        .setLatLng(e.latlng)
        .setContent("You clicked the map at " + e.latlng.toString())
        .openOn(mymap);
}



// mymap.on('click', onMapClick);
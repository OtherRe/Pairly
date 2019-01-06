var myChart = makeChart('myChart')

function drawChart(dataUrl) {
    const form = document.getElementById('get_data_form')
    if (!form.checkValidity()) return

    const httpClient = new HttpClient()
    const formData = new FormData(form)
    httpClient.getJSONWithArgs(dataUrl, formData.entries(), data => {
        myChart.data.labels = formatTimeForChart(data.time_axis);
        myChart.data.datasets[0].data = data.value_axis;
        myChart.update();
        scroll(0,0)
    });
}

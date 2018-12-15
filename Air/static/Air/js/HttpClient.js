var HttpClient = function () {
  /**
   *
   * @param {url to send GET request} aUrl
   * @param {*} aCallback
   */
  this.getJSON =
    function (aUrl, aCallback) {
      var anHttpRequest = new XMLHttpRequest();
      anHttpRequest.onload = function () {
        // if (anHttpRequest.readyState == 4 && anHttpRequest.status == 200)
        console.log('adding data')
        let data = JSON.parse(anHttpRequest.responseText)
        aCallback(data);
      }

      anHttpRequest.open('GET', aUrl, true);
      anHttpRequest.send(null);
    }

  this.getJSONWithArgs =
    function (aUrl, data, aCallback) {
      var anHttpRequest = new XMLHttpRequest();
      anHttpRequest.onload =
        function () {
          // if (anHttpRequest.readyState == 4 && anHttpRequest.status == 200)
          let data = JSON.parse(anHttpRequest.responseText)
          aCallback(data);
        }
      
      anHttpRequest.open('GET', aUrl + this.GETArgs(data), true);
      anHttpRequest.send(null);
    }
  this.GETArgs = data => {
    var args = "?"
    for(const pair of data)
      args += `${pair[0]}=${pair[1]}&`
    
    return args.substring(0, args.length - 1)
  }

  this.post = function (aUrl, errorCallback, aCallback, args) {
    let anHttpRequest = new XMLHttpRequest()
    anHttpRequest.onload = () => {
      if (anHttpRequest.readyState == 4 &&
        (anHttpRequest.status == 200 || anHttpRequest.status == 201))
        aCallback(anHttpRequest.responseText);
      else
        errorCallback(anHttpRequest.responseText)
    }
    let formData = new FormData()
    for (const key of Object.keys(args)) formData.append(key, args[key])

    anHttpRequest.open('POST', aUrl)

    anHttpRequest.send(formData)
  }
}
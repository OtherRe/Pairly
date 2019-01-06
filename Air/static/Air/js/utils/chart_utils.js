function makeChart(id){
  var ctx = document.getElementById(id);
  var myChart = new Chart(ctx, {
      type: 'line',
      data: {
          datasets: [{
              label: 'Time',
              data:[],
              backgroundColor: [
                  'rgba(255, 99, 132, 0.2)',
                  'rgba(54, 162, 235, 0.2)',
                  'rgba(255, 206, 86, 0.2)',
                  'rgba(75, 192, 192, 0.2)',
                  'rgba(153, 102, 255, 0.2)',
                  'rgba(255, 159, 64, 0.2)'
              ],
              lineTension:0,
              borderColor: [
                  'rgba(255,99,132,1)',
                  'rgba(54, 162, 235, 1)',
                  'rgba(255, 206, 86, 1)',
                  'rgba(75, 192, 192, 1)',
                  'rgba(153, 102, 255, 1)',
                  'rgba(255, 159, 64, 1)'
              ],
              borderWidth: 1
          }]
      },
      options: {
          scales: {
              yAxes: [{
                  ticks: {
                      beginAtZero:true
                  }
              }]
          }
      }
  });

  return myChart;
}

/**
 * 
 * @param {array containing data strings} data 
 * @returns array with removed redundant date information
 *        
 *
 */
function formatTimeForChart(dates){
  const time_axis = [dates[0]]

  for (let i = 1; i < dates.length; i++) {
      const [curr_date, curr_time] = dates[i].split(' ');
      const [prev_date, _] = dates[i - 1].split(' ');
      const [curr_year, curr_month, curr_day] = curr_date.split('-')
      const [prev_year, prev_month, prev_day] = prev_date.split('-')

      let curr = ''
      if(curr_year !== prev_year)
          curr = curr_year + curr_month + curr_day
      else if(curr_month !== prev_month)
          curr = curr_month + curr_day
      else if(curr_day !== prev_day)
          curr = curr_day
      
      //removing seconds from time
      curr += curr_time.split(':')
                        .slice(0,2)
                        .join(':')

      time_axis.push(curr)
  }
  return time_axis;
}
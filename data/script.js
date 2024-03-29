var chartT = new Highcharts.Chart({
    chart:{ renderTo : 'chart-temperature' },
    title: { text: 'DHT11 Temperature' },
    series: [{
      showInLegend: false,
      data: [10, 0, 84, 76, 0 , 60, 84, 76, 0]
      //data: [%STATE%]
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      },
      series: { color: '#059e8a' }
    },
    xAxis: {
      type: 'datetime',
      categories: ["2021-08-01", "2021-08-02", "2021-08-03", "2021-08-04", "2021-08-05", "2021-08-06", "2021-08-07", "2021-08-08", "2021-08-09"]
      //categories: [%MYSTATE%]
      //dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'Temperature (Celsius)' }
      //title: { text: 'Temperature (Fahrenheit)' }
    },
    credits: { enabled: false }
  }); 
    
    Highcharts.chart('chart-temperature', {
        credits: {
            enabled: false
        },
        chart: {
            zoomType: 'xy'
        },
        title: {
            text: 'Data collected trend',
            align: 'left'
        },
        subtitle: {
            text: 'Trend: ',
            align: 'left'
        },
        xAxis: [{
            categories: [],
            crosshair: true
        }],
        yAxis: [{ // Primary yAxis
            labels: {
                format: '{value}°C',
                style: {
                    color: Highcharts.getOptions().colors[1]
                }
            },
            title: {
                text: 'Temperature',
                style: {
                    color: Highcharts.getOptions().colors[1]
                }
            }
        }, { // Secondary yAxis
            title: {
                text: 'Humidity',
                style: {
                    color: Highcharts.getOptions().colors[0]
                }
            },
            labels: {
                format: '{value} &#37',
                style: {
                    color: Highcharts.getOptions().colors[0]
                }
            },
            opposite: true
        }],
        tooltip: {
            shared: true
        },
        legend: {
            layout: 'vertical',
            align: 'left',
            x: 80,
            verticalAlign: 'top',
            y: 55,
            floating: true,
            backgroundColor:
                Highcharts.defaultOptions.legend.backgroundColor || // theme
                'rgba(255,255,255,0.25)'
        },
        series: [{
            name: 'Humidity',
            type: 'column',
            yAxis: 1,
            data: [],
            tooltip: {
                valueSuffix: ' &#37'
            }
    
        }, {
            name: 'Temperature',
            type: 'spline',
            data: [],
            tooltip: {
                valueSuffix: '°C'
            }
        }]
    });

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {

            //console.log(this.responseText);
            const my_array = JSON.parse(this.responseText);
            //const my_array =  JSON.parse('{"labels":["2021-08-01", "2021-08-02", "2021-08-03", "2021-08-04", "2021-08-05", "2021-08-02", "2021-08-03", "2021-08-04", "2021-08-05"], "temperature":[0, 0, 84, 76, 0 , 0, 84, 76, 0], "humidity":[1, 1, 1, 1, 0, 1, 1, 1, 0]}');
            document.getElementById("avg_temperature").innerHTML = my_array.avg_temperature;
            document.getElementById("avg_humidity").innerHTML = my_array.avg_humidity;

            Highcharts.chart('chart-temperature', {
                credits: {
                    enabled: false
                },
                chart: {
                    zoomType: 'xy'
                },
                title: {
                    text: 'Data collected trend',
                    align: 'left'
                },
                subtitle: {
                    text: 'Trend: ',
                    align: 'left'
                },
                xAxis: [{
                    categories: my_array.labels,
                    crosshair: true
                }],
                yAxis: [{ // Primary yAxis
                    labels: {
                        format: '{value}°C',
                        style: {
                            color: Highcharts.getOptions().colors[1]
                        }
                    },
                    title: {
                        text: 'Temperature',
                        style: {
                            color: Highcharts.getOptions().colors[1]
                        }
                    }
                }, { // Secondary yAxis
                    title: {
                        text: 'Humidity',
                        style: {
                            color: Highcharts.getOptions().colors[0]
                        }
                    },
                    labels: {
                        format: '{value} &#37',
                        style: {
                            color: Highcharts.getOptions().colors[0]
                        }
                    },
                    opposite: true
                }],
                tooltip: {
                    shared: true
                },
                legend: {
                    layout: 'vertical',
                    align: 'left',
                    x: 80,
                    verticalAlign: 'top',
                    y: 55,
                    floating: true,
                    backgroundColor:
                        Highcharts.defaultOptions.legend.backgroundColor || // theme
                        'rgba(255,255,255,0.25)'
                },
                series: [{
                    name: 'Humidity',
                    type: 'column',
                    yAxis: 1,
                    data: my_array.humidity,
                    tooltip: {
                        valueSuffix: ' &#37'
                    }
            
                }, {
                    name: 'Temperature',
                    type: 'spline',
                    data: my_array.temperature,
                    tooltip: {
                        valueSuffix: '°C'
                    }
                }]
            });



          }
        };
        xhttp.open("GET", "/update_graph", true);
        xhttp.send();
      }, 10000 ) ;   

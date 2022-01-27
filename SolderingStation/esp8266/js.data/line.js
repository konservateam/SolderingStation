randomScalingFactor = function()
{
  return Math.round(Math.random() * 100);
};

var config = {
    type: 'line',
        data: {
            labels: ["Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль"],
            datasets: [{
                label: "Dataset1",
                fill: false,
                backgroundColor: chartColors.red,
                borderColor: chartColors.red,
                data: [
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor()
                ]
            }, {
                label: "Dataset2",
                fill: false,
                backgroundColor: chartColors.blue,
                borderColor: chartColors.blue,
                data: [
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor(),
                    randomScalingFactor()
                ]
            }]
        },
        options: {
            responsive: true,
            title:{
                display:true,
                text:'Line Chart (Chart.js)'
            },
      	    legend: {
    		    position: 'bottom',
    	    },
            tooltips: {
                mode: 'index',
                intersect: false,
            },
            hover: {
                mode: 'nearest',
                intersect: true
            },
            scales: {
                xAxes: [{
                    display: true,
                    scaleLabel: {
                        display: true,
                        labelString: 'Месяц'
                    }
                }],
                yAxes: [{
                    display: true,
                    scaleLabel: {
                        display: true,
                        labelString: 'Значение'
                    }
                }]
            }
        }
};
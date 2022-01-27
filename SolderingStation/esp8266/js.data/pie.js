randomScalingFactor = function()
{
  return Math.round(Math.random() * 100);
};

var config = {
  type: 'pie',
  data: {
    datasets: [{
      data: [
        randomScalingFactor(),
        randomScalingFactor(),
        randomScalingFactor(),
        randomScalingFactor(),
        randomScalingFactor(),
      ],
      backgroundColor: [
        chartColors.red,
        chartColors.orange,
        chartColors.yellow,
        chartColors.green,
        chartColors.blue,
      ],
      label: 'Dataset 1'
    }],
    labels: [
        "Red",
        "Orange",
        "Yellow",
        "Green",
        "Blue"
    ]
  },
  options: {
	  responsive: true,
	  legend: {
		  position: 'bottom',
	  }
  }
};
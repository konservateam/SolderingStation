randomScalingFactor = function()
{
	return (Math.random() > 0.5 ? 1.0 : -1.0) * Math.round(Math.random() * 100);
};

var barChartData = {
    labels: ["Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль"],
    datasets: [{
    	label: 'Dataset 1',
    	backgroundColor: color(chartColors.red).alpha(0.5).rgbString(),
    	borderColor: chartColors.red,
    	borderWidth: 1,
    	data: [
    	       randomScalingFactor(),
    	       randomScalingFactor(),
    	       randomScalingFactor(),
    	       randomScalingFactor(),
    	       randomScalingFactor(),
    	       randomScalingFactor(),
    	       randomScalingFactor()
    	]
    }, 
    {
    	label: 'Dataset 2',
    	backgroundColor: color(chartColors.blue).alpha(0.5).rgbString(),
    	borderColor: chartColors.blue,
    	borderWidth: 1,
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
};

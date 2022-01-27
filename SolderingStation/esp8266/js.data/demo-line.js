randomScalingFactor = function() {
    return Math.round(Math.random() * 100);
};

barChartData = {
    labels: ["0", "50", "100", "150", "200", "250", "300", 
             "350","400", "450", "500", "550","600"],
    datasets: [{
    	label: 'Dataset1',
    	fill: false,
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
    	label: 'Dataset2',
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
               randomScalingFactor(), 
               randomScalingFactor(), 
               randomScalingFactor(),
               randomScalingFactor(), 
               randomScalingFactor(), 
               randomScalingFactor(), 
               randomScalingFactor()
        ]
	}],
    options: {
        scales: {
            y: {
                suggestedMin: 50,
                suggestedMax: 150
            }
        }
    }
};

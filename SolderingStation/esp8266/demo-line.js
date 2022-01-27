randomScalingFactor = function() {
    return Math.round(Math.random() * 100);
};

barChartData = {
    labels: [["Июнь","2015"], "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь", 
             ["Январь","2016"],"Февраль", "Март", "Апрель", "Май"],
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
               randomScalingFactor()
        ]
	}]
};

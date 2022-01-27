randomScalingFactor = function() {
    return Math.round(Math.random() * 100);
};

barChartData = {
    labels: ["600", "550", "500", "450", 
                            "400", "350", "300", 
             "250","200", "150", "100", "50","0"],
    datasets: [{
        label: 'Dataset1',
        fill: false,
        backgroundColor: color(chartColors.red).alpha(0.5)
                                               .rgbString(),
        borderColor: chartColors.red,
        borderWidth: 1,
        data: [
               randomScalingFactor(), 
               randomScalingFactor(), 
               . . .
        ]
    }, 
    {
        label: 'Dataset2',
        backgroundColor: color(chartColors.blue).alpha(0.5)
                                                .rgbString(),
        borderColor: chartColors.blue,
        borderWidth: 1,
        data: [
               randomScalingFactor(),
               randomScalingFactor(),
               . . .
        ]
    }]
};
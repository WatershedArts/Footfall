// * Name: main.js
// * Project: Footfall
// * Author: David Haylock & Oliver Humpage
// * Creation Date: 20-11-2014
// * Copyright: (c) 2015 by Watershed Arts Trust Ltd.

var hostUrl = "http://localhost/upload.php";
var trafficData;
var totalsData;
var trafficChart;
var totalChart;
var interval = 300;
var events;
var labelLength = 0;
//--------------------------------------------------------------
// *
// *	Generate Legend For Charts
// *
//--------------------------------------------------------------
function generateChartLegend(parent, data)
{
		parent.className = 'legend';
		var datas = data.hasOwnProperty('datasets') ? data.datasets : data;

		// remove possible children of the parent
		while(parent.hasChildNodes())
		{
			parent.removeChild(parent.lastChild);
		}

		datas.forEach(function(d)
		{
			var title = document.createElement('span');
			title.className = 'title';
			title.style.borderColor = d.hasOwnProperty('strokeColor') ? d.strokeColor : d.color;
			title.style.borderStyle = 'solid';
			parent.appendChild(title);

			var text = document.createTextNode(d.label);
			title.appendChild(text);
		});
}
//--------------------------------------------------------------
// *
// *	Fill the chart with existing data
// *
//--------------------------------------------------------------
function preLoadData()
{
	// Get the First Load of Data
	$.ajax({
		url:hostUrl+'?get&interval='+interval,
		async: true,
		dataType: 'json',
		type:'get',
	}).done(function(data){

		// If we already have data update the labels on the web page
		updateLabels(data);
		for (var i in data) {
			labelLength++;
			var label = data[i].timekey.substring(11,16);
			var eventsDatasets = [];
			eventsDatasets.push(data[i].total);
			eventCount = 0;
			for (var title in events) {
				eventAtCurrentLabel = null;
				var ev = events[title];
				//console.log(title);
				for (var t in ev) {
					if (ev[t].start <= label && ev[t].end >= label) {
						//console.log(title+" in label "+label);
						eventAtCurrentLabel = -10-(eventCount*10);
					}
				}
				eventsDatasets.push(eventAtCurrentLabel);
				eventCount++;
			}
			// Best way of getting the data into the graphs | substring cuts the timestamp down
			totalChart.addData(eventsDatasets,label);
			trafficChart.addData([data[i].peoplein, -data[i].peopleout],data[i].timekey.substring(11,16));
		}
	});
}
//--------------------------------------------------------------
// *
// *	Create Total Chart
// *
//--------------------------------------------------------------
function createTotalChart()
{
	var canvas = document.getElementById('totalChart').getContext('2d');
    // Total Data object
	events = {};
		var datasets = [{
					label: "People In Watershed",
					fillColor: "rgba(0,77,255,.01)",
					strokeColor: "rgba(0,77,255,1)",
					pointColor: "rgba(0,77,255,1)",
					pointStrokeColor: "#fff",
					data: [0,0]
			}];
		var eventCount = 0;
		$.each(events, function(title,ev){
			var color = "rgba("+(eventCount*30)+","+(255-(eventCount*40))+","+(eventCount*30)+",1)";

			datasets.push({
						label: title,
						fillColor: "rgba(0,77,255,.01)",
						strokeColor: color,
						pointColor: "rgba(0,77,255,.01)",
						pointStrokeColor: "rgba(0,77,255,.01)",
						strokeWidth: 5,
						data: [null,null]
			});
			eventCount++;
		});
		totalsData = {
				labels: ['0','08:00'],
				datasets: datasets
		};

		// Chart Options
		var options = {
			animation: false,
			responsive:true,
			scaleShowLabels: 20,
			scaleBeginAtZero : false,
			skipXLabels:true,
			bezierCurveTension : 0.3,
			pointHitDetectionRadius : 10,
			scaleGridLineColor : "rgba(0,0,0,.1)",
			scaleGridLineWidth : 2,
			bezierCurve : true
		};

		// Make Chart
		totalChart = new Chart(canvas).Line(totalsData, options);
		preLoadData();
}
//--------------------------------------------------------------
// *
// *	Create People IN & OUT Chart
// *
//--------------------------------------------------------------
function createTrafficChart()
{
	var canvas = document.getElementById('trafficChart').getContext('2d');

    trafficData = {
		labels: ['08:00'],
		datasets:
		[
			{
				label: "Traffic In",
				fillColor: "rgba(65,190,242,0.5)",
				strokeColor: "rgba(65,190,242,0.5)",
				pointColor: "rgba(65,190,242,0.5)",
				pointStrokeColor: "#fff",
				data: [0],
				chartType: 'Bar'
			},
			{
				label: "Traffic Out",
				fillColor: "rgba(242,65,65,0.5)",
				strokeColor: "rgba(242,65,65,0.5)",
				pointColor: "rgba(242,65,65,0.5)",
				pointStrokeColor: "#fff",
				data: [0],
				chartType: 'Bar'
			}
		]
    };

    // Chart Options
    var options = {
		animation: false,
		responsive:true,
		scaleGridLineColor : "rgba(0,0,0,.1)",
		// Important: allows the chart to draw both scales + / -
		barBeginAtOrigin: true,
		scaleBeginAtZero : false
    };
	// Make Chart
	trafficChart = new Chart(canvas).Bar(trafficData,options);
	generateChartLegend(document.getElementById("trafficLegend"),trafficData);
}
//--------------------------------------------------------------
// *
// *	Function to Update Labels
// *
//--------------------------------------------------------------
function updateLabels(data)
{
	document.getElementById("peopleIn").innerHTML = data[data.length-1].peoplein;
	document.getElementById("peopleOut").innerHTML = -data[data.length-1].peopleout;
	document.getElementById("currentNumber").innerHTML = data[data.length-1].total;
	document.getElementById("totalIn").innerHTML = data[data.length-1].totalin;
}
//--------------------------------------------------------------
// *
// *	Function to Update End Value of Totals Chart
// *
//--------------------------------------------------------------
function updateTotals(data)
{
	//console.log("Updating totals");
	var totalDataLength = totalChart.datasets[0].points.length-1;
	totalChart.datasets[0].points[totalDataLength].value = data[data.length-1].total;
	if (totalDataLength-1 > labelLength) {
		var label = totalChart.datasets[0].points[totalDataLength].label;
		var eventCount = 0;
		for (var title in events) {
			eventAtCurrentLabel = null;
			var ev = events[title];
			for (var t in ev) {
				if (ev[t].start <= label && ev[t].end >= label) {
					eventAtCurrentLabel = -10-(eventCount*10);
				}
			}
			totalChart.datasets[eventCount+1].points[totalDataLength].value = eventAtCurrentLabel;
			eventCount++;
		}
	}
	totalChart.update();
}
//--------------------------------------------------------------
// *
// *	Function to Update End Value of Traffic Chart
// *
//--------------------------------------------------------------
function updatePeople(data)
{
	var trafficInDataLength = trafficChart.datasets[0].bars.length-1;
	var trafficOutDataLength = trafficChart.datasets[1].bars.length-1;
	trafficChart.datasets[0].bars[trafficInDataLength].value = data[data.length-1].peoplein;
	trafficChart.datasets[1].bars[trafficOutDataLength].value = -data[data.length-1].peopleout;
	trafficChart.update();
}
//--------------------------------------------------------------
// *
// *	Function which checks for New data
// *
//--------------------------------------------------------------
function updateValues()
{
	$.ajax({
		url:hostUrl+'?get&interval'+interval,
		async:true,
		dataType: 'json',
		type:'get',
	}).done(function(data)
	{
		updateLabels(data);
		updateTotals(data);
		updatePeople(data);
		addDataToCharts(data);
	});
}
//--------------------------------------------------------------
// *
// *	Function to Add Data to Charts
// *
//--------------------------------------------------------------
function addDataToCharts(data)
{
	// Probably a better way to do this
	if (data[data.length-1].timekey.substring(11,16) == trafficData.labels[trafficData.labels.length-1])
	{

	}
	else
	{
		trafficChart.addData([data[data.length-1].peoplein,-data[data.length-1].peopleout],data[data.length-1].timekey.substring(11,16));
		var trafficInDataLength = trafficChart.datasets[0].bars.length-2;
		var trafficOutDataLength = trafficChart.datasets[1].bars.length-2;

		trafficChart.datasets[0].bars[trafficInDataLength].value = data[data.length-2].peoplein;
		trafficChart.datasets[1].bars[trafficOutDataLength].value = -data[data.length-2].peopleout;
		trafficChart.update();
	}
	if (data[data.length-1].timekey.substring(11,16) == totalsData.labels[totalsData.labels.length-1])
	{

	}
	else
	{
		totalChart.addData([data[data.length-1].total],data[data.length-1].timekey.substring(11,16));
		var totalDataLength = totalChart.datasets[0].points.length-2;
		totalChart.datasets[0].points[totalDataLength].value = data[data.length-2].total;
		totalChart.update();
	}
}
//--------------------------------------------------------------
// *
// *	Main
// *
//--------------------------------------------------------------
$(document).ready(function(){
	interval = $('#interval').val();
	$('#interval').change(function(){
		interval = $('#interval').val();
		trafficChart.destroy();
		totalChart.destroy();
		createTrafficChart();
		createTotalChart();
	});

	// Functions
	createTrafficChart();
	createTotalChart();
	setInterval(function(){updateValues()},5000);
});

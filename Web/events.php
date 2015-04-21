<!DOCTYPE html>
<html>
	<head>
		<title>Footfall</title>
		<link rel="stylesheet" type="text/css" href="stylesheet.css"></link>
		<script src="//code.jquery.com/jquery-1.11.0.min.js"></script>
		<link rel="stylesheet" href="//code.jquery.com/ui/1.11.2/themes/smoothness/jquery-ui.css">
		<script src="//code.jquery.com/jquery-1.10.2.js"></script>
		<script src="//code.jquery.com/ui/1.11.2/jquery-ui.js"></script>
		<script src='Chart.js'></script>
		<script type="text/javascript"></script>
		<script src='main-events.js'></script>
	</head>
		<div class="mainDiv">
			<h2>Today's Footfall</h2>
			<select id='interval'>
				<option value="300">5 mins</option>
				<option value="600">10 mins</option>
				<option value="900" selected>15 mins</option>
				<option value="1800">30 mins</option>
				<option value="3600">1 hour</option>
			</select>
			<div>
				<div>
					<h3>Total</h3>
					<div class="graphs" align="center">
						<div style="margin-top: -60px; margin-bottom: -60px;">
							<h4 align="center" style="display:inline-flex;">Visitors Today:</h4>
							<h5 id="totalIn" style='margin-right: 10px;' class="numbers">0</h5>
							<h4 align="center" style="display:inline-flex;">Number of People Currently in The Building:</h4>
							<h5 id="currentNumber" class="numbers">0</h5>
						</div>
						<canvas style='margin-left: 5px;' id="totalChart" width='900' height='300' ></canvas>
						<div style='margin-bottom: 10px;' id="totalLegend"></div>
					</div>
				</div>
				<div>
					<h3>Traffic</h3>
					<div class="graphs" align="center">
						<div style="margin-top: -60px; margin-bottom: -60px;">
							<h4 class="people">Traffic In:</h4>
							<h5 id="peopleIn" class="numbers">0</h5>
							<h4 class="people">Traffic Out:</h4>
							<h5 id="peopleOut" class="numbers">0</h5>
						</div>
						<canvas id="trafficChart" width='900' height='300'></canvas>
						<div style='margin-bottom: 10px;' id="trafficLegend"></div>
					</div>
				</div>
			</div>
		</div>
	</body>
</html>
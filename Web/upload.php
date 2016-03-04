<?php

require_once('includes/dbconnect.php');

if (ini_get('magic_quotes_gpc')) {
	foreach ($_GET as $k => $v) {
		$_GET[$k] = stripslashes($v);
	}
	foreach ($_POST as $k => $v) {
		$_POST[$k] = stripslashes($v);
	}
}
//
function sanitise_filename($filename) {
	return basename(preg_replace("/[^0-9a-z._-]/i", "", $filename));
}

if (isset($_POST['submit'])) {
		// Require Secret Key
		require_once('includes/secret.php');

		// Get Values
		$location = $_POST['location'];
		$count = $_POST['count'];
		$rawtimestamp = $_POST['rawtimestamp'];

		// Prepare INSERT QUERY
		$sqlq = "INSERT INTO `data` (`timestamp`,`locationID`,`event`) VALUES (:rawtimestamp,:location,:count)";
		$q = $DBH->prepare($sqlq);
		$q->execute(array(':rawtimestamp' => $rawtimestamp, ':count' => $count, ':location' => $location));

		// Error Handling
		if (!$q) {
			echo "Error: can't insert into database. ".$q->errorCode();
			exit;
		}
		else {
			echo "Count: " .$count;
			echo "</br>";
			echo "Location: " . $location;
			echo "</br>";
			echo "Raw Timestamp: " . $rawtimestamp;
			exit;
		}
}
elseif (isset($_GET['get'])){
	if (isset($_GET['interval']) && intval($_GET['interval'])) {
		$interval = intval($_GET['interval']);
	}
	else {
		// Default interval of 1 hour
		$interval = 3600;
	}
	$query = "
				SELECT  FROM_UNIXTIME(FLOOR(UNIX_TIMESTAMP(timestamp)/".$interval.")*".$interval.") AS timekey,
				SUM(event) as movement,
				SUM(IF(event >0, event, 0)) as peoplein,
				SUM(IF(event < 0, ABS(event), 0)) as peopleout
				FROM     data
				WHERE DATE(timestamp) = DATE(NOW())
				GROUP BY timekey
				ORDER BY timekey ASC
			";

	$get = $DBH->prepare($query);
	$get->execute();

	if (!$get) {
		echo "Error: couldn't execute query. ".$get->errorCode();
		exit;
	}

	if ($get->fetchColumn() == 0) {
		echo "[]";
		exit;
	}
	$rows = array();
	$runningtotal = 0;
	$runningtotalin = 0;
	$rowCount = 0;
	$totalin = 0;
	while ($row = $get->fetch(PDO::FETCH_ASSOC)) {
		$runningtotal += $row['movement'];
		$row['total'] = $runningtotal;
		$totalin += $row['peoplein'];
		$row['totalin'] = $totalin;
		$rows[$row['timekey']] = $row;
		if ($rowCount == 0) {
			$starttime = strtotime($row['timekey']);
		}
		$rowCount++;
	}
	$endtime = time() - (time() % $interval);
	for ($t = $starttime; $t <= $endtime; $t += $interval) {
		$dt = date("Y-m-d H:i:s",$t);
		if (!isset($rows[$dt])) {
			$rows[$dt] = array("timekey" => $dt, "movement" => 0, "peoplein" => 0, "peopleout" => 0, "total" => $runningtotal, "totalin" => $runningtotalin);
		}
		else {
			$runningtotal = $rows[$dt]['total'];
			$runningtotalin = $rows[$dt]['totalin'];
		}
	}
	ksort($rows); // sort
	$rows = array_values($rows); // change back into indexed

	echo json_encode($rows);
}
?>

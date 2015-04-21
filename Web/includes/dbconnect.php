<?php
	$dbname = "<Your-Database-Name>";
	$passkey= "<Your-Database-Password?>";
	$host = "localhost";
	try {
		$DBH = new PDO('mysql:host=localhost;dbname=<Your-Database-Name>',$dbname,$passkey);
	} catch (PDOException $e) {
		echo $e->getMessage();
		exit;
	}
?>
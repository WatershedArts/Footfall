<?php
	$dbname = "root";
	$passkey= "root";
	$host = "localhost";
	try {
		$DBH = new PDO('mysql:host=localhost;dbname=footfall',$dbname,$passkey);
	} catch (PDOException $e) {
		echo $e->getMessage();
		exit;
	}
?>

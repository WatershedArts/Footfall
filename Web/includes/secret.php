<?php
$secret = "<A-Secret-Key>";
if (!isset($_POST['secret']) || $_POST['secret'] != $secret) {
	header('HTTP/1.0 403 Forbidden');
	exit;
}
?>
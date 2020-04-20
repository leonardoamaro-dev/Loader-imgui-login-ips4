<?php
	$conn = new mysqli('localhost', 'db_user', 'db_pass', 'cb_name');

	if ($conn->connect_error) {

	    die("Connection failed: " . $conn->connect_error);

	}
?>
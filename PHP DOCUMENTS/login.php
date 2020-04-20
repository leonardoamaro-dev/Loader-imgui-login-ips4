<?php
	session_start();
	include 'conn.php';

	if(isset($_POST['login'])){
		$username = $_POST['username'];
		$password = $_POST['password'];

		$sql = "SELECT * FROM core_members WHERE name = '$username'";
		$query = $conn->query($sql);

		if($query->num_rows < 1){
			echo 'incorrect';
		}
		else{
			$row = $query->fetch_assoc();
			if(password_verify($password, $row['members_pass_hash'])){
				echo 'true';
			}
			else{
				echo 'incorrect';
			}
		}
		
	}
	else{
		echo 'post não recebido';
	}
?>
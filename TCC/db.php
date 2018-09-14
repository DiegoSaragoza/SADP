<!DOCTYPE html>
<html>
<head>
	<title>Teste BD</title>
</head>
<body>
	<?php
		$mysql = new mysql("localhost", "root", "etecia", "bdloja");
		if (!$mysql) {
   			die("Connection failed: " . $mysql->connect_error);
		}
		echo "Connected Succefully";
	?>

	<p>Teste de Conexão com banco de dados</p>
	
</body>
</html>



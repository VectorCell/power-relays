<?php

	function parsefile ($filename)
	{
		$retval = array();
		$contents = file_get_contents("pins.conf");
		$lines = explode(PHP_EOL, $contents);
		foreach ($lines as $line) {
			if ($line[0] != '#' && strlen($line) > 0) {
				$pinnum = strtok($line, " ");
				$onstate = strtok(" ");
				$pinname = strtok(" ");
				array_push($retval, array($pinname, $pinnum, $onstate));
			}
		}
		return $retval;
	}

	function get_state_image ($state)
	{
		if ($state) {
			return "green.jpg";
		} else {
			return "red.jpg";
		}
	}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta charset="utf-8" />
		<title>Power Relays</title>
		<script src="changepin.js"></script>
	</head>
	<body style="color: white; background-color: black;">
		<?php
			$pinarr = parsefile("pins.conf");
			echo "<table>\n";
			foreach ($pinarr as $pininfo) {
				echo "<tr>\n";
				$pinname = $pininfo[0];
				$pinnum  = $pininfo[1];
				$onstate = $pininfo[2];
				$pinon = false;
				if ($onstate === exec("gpio read " . $pinnum)) {
					$pinon = true;
				}
				echo "<td><img id='button" . $pinnum
					. "' src='img/" . get_state_image($pinon)
					. "' onclick='changepin(" . $pinnum . ", " . $onstate . ");'></td>\n";
				echo "<td><h1 style='font-size: 96pt; font-family: monospace;'>" . strtoupper($pinname) . "</h1></td>\n";
				echo "</tr>\n";
				echo "\n";
			}
			echo "</table>\n";
		?>
		<pre><?php
//			$pinarr = parsefile("pins.conf");
//			foreach ($pinarr as $pininfo) {
//				$pinname = $pininfo[0];
//				$pinnum  = $pininfo[1];
//				$onstate = $pininfo[2];
//				echo "pinname: " . $pinname . "\n";
//				echo "pinnum:  " . $pinnum  . "\n";
//				echo "onstate: " . $onstate . "\n";
//				echo "state:   " . exec("gpio read " . $pinnum) . "\n";
//				if ($onstate === exec("gpio read " . $pinnum)) {
//					echo "ON\n";
//				} else {
//					echo "OFF\n";
//				}
//				echo "\n";
//			}
		?></pre>
	</body>
</html>

<?php

	if (!isset($_GET["pinnum"]) || !isset($_GET["onstate"])) {
		echo "fail";
	} else {
		$pinnum  = $_GET["pinnum"];
		$onstate = $_GET["onstate"];

		if (!is_numeric($pinnum) || !is_numeric($onstate)) {
			echo "fail";
		} else {
			system("gpio mode " . $pinnum . " out");
			exec("gpio read " . $pinnum, $status, $return);
			if ($status[0] === "0") {
				$status[0] = "1";
			} else if ($status[0] === "1") {
				$status[0] = "0";
			}
			system("gpio write " . $pinnum . " " . $status[0]);
			exec("gpio read " . $pinum, $status, $return);
			if ($status[0] === $onstate) {
				echo "1";
			} else {
				echo "0";
			}
		}
	}

?>

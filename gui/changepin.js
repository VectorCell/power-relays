function changepin (pinnum, onstate) {

	button_id = "button" + pinnum;

	var button = document.getElementById(button_id);

	var data = 0;

	// send the pin number to gpio.php for changes
	// this is the http request
	var request = new XMLHttpRequest();
	request.open("GET", "gpio.php?pinnum=" + pinnum + "&onstate=" + onstate, true);
	request.send(null);

	// receiving information
	request.onreadystatechange = function () {
		if (request.readyState == 4 && request.status == 200) {
			data = request.responseText;

			// update the index pic
			if ( !(data.localeCompare("0")) ){
				button.src = "img/red.jpg";
			} else if ( !(data.localeCompare("1")) ) {
				button.src = "img/green.jpg";
			} else if ( !(data.localeCompare("fail"))) {
				alert ("ERROR: code 1" );
				return ("fail");			
			} else {
				alert ("ERROR: code 2" );
				return ("fail"); 
			}
		} else if (request.readyState == 4 && request.status == 500) {
			alert ("server error");
			return ("fail");
		} else if (request.readyState == 4 && request.status != 200 && request.status != 500 ) { 
			alert ("ERROR: code 3");
			return ("fail");
		}
	}	
	return 0;
}

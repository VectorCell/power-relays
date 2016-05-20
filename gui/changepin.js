function changepin (pinnum, onstate) {

	button_id = "button" + pinnum;

	var button = document.getElementById(button_id);

	var data = 0;

	//send the pic number to gpio.php for changes
	//this is the http request
	var request = new XMLHttpRequest();
	request.open("GET", "gpio.php?pinnum=" + pinnum + "&onstate=" + onstate, true);
	request.send(null);

	//receiving informations
	request.onreadystatechange = function () {
		if (request.readyState == 4 && request.status == 200) {
			data = request.responseText;

			//update the index pic
			if ( !(data.localeCompare("0")) ){
				button.src = "img/red.jpg";
			}
			else if ( !(data.localeCompare("1")) ) {
				button.src = "img/green.jpg";
			}
			else if ( !(data.localeCompare("fail"))) {
				alert ("Something went wrong!" );
				return ("fail");			
			}
			else {
				alert ("Something went wrong!" );
				return ("fail"); 
			}
		}
		//test if fail
		else if (request.readyState == 4 && request.status == 500) {
			alert ("server error");
			return ("fail");
		}
		//else 
		else if (request.readyState == 4 && request.status != 200 && request.status != 500 ) { 
			alert ("Something went wrong!");
			return ("fail"); }
	}	
	return 0;
}
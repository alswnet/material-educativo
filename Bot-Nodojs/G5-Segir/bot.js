console.log('Iniciar el bot');

var Twit = require('twit');

tweetIt();
setInterval(tweetIt, 1000*20);

function tweetIt() {

	var r = Math.floor(Math.random()*100);

	var tweet = {
	  status: 'Numero Random ' + r + ' #ALSWbot'
	}

	T.post('statuses/update', tweet, tweeted);

	function tweeted(err, data, response) {
	  if (err) {
	  	console.log("Algo Salio Mal");
	  } else {
	    console.log("Funciono");
	  }
	}
}

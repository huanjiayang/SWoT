

function requestresult() {
	
	$.ajax({
		url: "/update/"+ebookname + '/',
		type: 'GET',
		accepts: 'application/json',
		success: function(data, textStatus, jqXHR) {
			if(data[0].finished == true)
				$('#'+data[0].position).html('response data: ' + data[0].result);
		},
		failure: function() {
			alert('Unable to retrieve list of ebooks');
		}
	});

}
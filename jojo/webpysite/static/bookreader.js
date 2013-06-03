function requestresult() {
	use_cache = (document.location.hash == '#cache');
	$.ajax({
		url: "/update/"+ebookname + '/',
        data: "use_cache=" + use_cache,
		type: 'GET',
		accepts: 'application/json',
		success: function(data, textStatus, jqXHR) {
			if(data[0].finished == true)
				$('#'+data[0].position).html(data[0].result);
		},
		failure: function() {
			alert('Unable to retrieve list of ebooks');
		}
	});

}


function query(){
	
	var url = '/query/';
	var x = document.getElementById('query_type').selectedIndex;
	var querytype = document.getElementById('query_type')[x].value;
	var queryvalue = querytype = document.getElementById('query_input').value;
	
	url = url + '?' + querytype + '=' + queryvalue;
	window.location.href = url;
/*	
	$.ajax({
		url: url,
		type: 'GET',
		accepts: 'application/json',
		success: function(data, textStatus, jqXHR) {
			//alert(data[0].creator);
			document.getElementById('query_result').innerHTML = data;
			for(i=0;i<=data.length;i++)
				for(key in data[i])
					alert(data[i][key]);
			
		},
		failure: function() {
			alert('Unable to retrieve information of the ebooks');
		}
	});
*/	
}
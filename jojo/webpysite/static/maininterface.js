function getebooks() {

	$.ajax({
		url: "/ebooklist/",
		type: 'GET',
		accepts: 'application/json',
		success: function(data, textStatus, jqXHR) {
			var x;
			var ebook;
			// refer to the html select box
			var ebookslist = document.getElementById("ebooklist");
			// clear the html select box 
			for(x=0; x<ebookslist.options.length; x++) {
				ebookslist.options[x] = null;
			}
			// put data received into the html select box
			for(x=0; x<data.length; x++) {
				ebook = data[x];
				ebookslist.options[x] = new Option(ebook.title, ebook.url), false, false;
			}
		},
		failure: function() {
			alert('Unable to retrieve list of ebooks');
		}
	});
}
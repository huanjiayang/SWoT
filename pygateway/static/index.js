function startQuery(){
	var selected = document.getElementById('selectbox').selectedIndex;
	var selected_id = document.getElementById('selectbox').children[selected].innerHTML;
	var xml_string = "<Request><submitter><name>client</name></submitter><queries><data><key>";
	xml_string += selected_id;
	xml_string += "</key><value>*</value></data></queries></request>";
	
		$.ajax({
		url: "/query/",
		type: 'GET',
	    data     : { xml: xml_string },
	    //dataType : "text/xml",
		accepts: 'application/xml',
		success: function(data, textStatus, jqXHR) {
	    	//alert('got info');
	    	test = data;
	    	document.getElementById('testdiv').value = '';
	    	var s = (new XMLSerializer()).serializeToString(data);
			document.getElementById('testdiv').value = s;
			
		},
		failure: function() {
			alert('Unable to start query');
		}
	});
		
		
	
	//alert('start query');
	//var rps = document.getElementById('mytextbox').value;
	//document.getElementById('testdiv').innerHTML = rps;
}
function List(){
		$.ajax({
		url: "/list/",
		type: 'GET',
		accepts: 'application/xml',
		success: function(data, textStatus, jqXHR) {
			//var rps = document.getElementById('mytextbox').value;
			//document.getElementById('testdiv').innerHTML = data;
			//alert(data);
			//test = data;
			if (window.DOMParser)
			  {
			  //var parser=new DOMParser();
			  //xmlDoc=parser.parseFromString(data,"text/xml");
			  document.getElementById('selectbox').innerHTML = '';
			  dataDOMs = data.getElementsByTagName("data");
			  
				  for(var i=0;i<dataDOMs.length;i++){
					  var optionHTML = '<option>'+dataDOMs[i].children[0].innerHTML+'</option>';
					  document.getElementById('selectbox').innerHTML += optionHTML;
				  }
			  }
			else
				{
				alert('no DOMParser');
				}

		},
		failure: function() {
			alert('Unable to fetch data from database!');
		}
	});
	
}
function startErase(){
	
document.getElementById('testdiv').value = '';
}
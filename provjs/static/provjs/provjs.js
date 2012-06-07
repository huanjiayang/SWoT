function fromJSONtoHTML(provjson)
{	var html = "";
	for (var key in provjson) {
		html = html + key + ":{<br>";
		if (key=="prefix"){
			for (var prefix in provjson.prefix)
				html = html + "&nbsp;&nbsp;&nbsp;&nbsp;"+prefix+" : " + provjson.prefix[prefix]+"<br>";
		}
		else if (key=="account"){
			for (var account in provjson.account)
				html = html + "&nbsp;&nbsp;&nbsp;&nbsp;"+account+" : " + fromJSONtoHTML(provjson.account[account])+"<br>";
		}
		else{
			for (var record in provjson[key]){
				html = html + "&nbsp;&nbsp;&nbsp;&nbsp;"+record+" : <br>";
				for (var attr in provjson[key][record]){
					html = html + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
					html = html + attr +" : "+provjson[key][record][attr]+"<br>";
				}
			}
		}
		html = html + "}<br><br><p>";
		}
	return html
}


function PROVLiteral(value,type){
	this.value = value;
	this.type = type;
}

function PROVLiteral_equals(x,y){
	if ( x === y ) return true;
	else return ((x.value == this.value)&&(x.type==this.type));
}


function provjs(json){
	this.json = json;
	this._visitedrecord = [];
	this._queriedrecord = [];
	this._provdmterms = {};
	this._provdmterms["element"] = ["activity",
	                                "agent",
			                        "entity",
			                        "note"];
	this._provdmterms["relation"] = ["used",
		                             "actedOnBehalfOf",
		                             "alternateOf",
		                             "hasAnnotation",
		                             "specializationOf",
		                             "wasAssociatedWith",
		                             "wasDerivedFrom",
		                             "wasEndedBy",
		                             "wasGeneratedBy",
		                             "wasStartedBy",
		                             "tracedTo",
		                             "wasInformedBy",
		                             "wasRevisionOf",
		                             "wasAttributedTo",
		                             "wasQuotedFrom",
		                             "wasSummaryOf",
		                             "hadOriginalSource"];
	this._provdmterms["record"] = ["account"].concat(this._provdmterms["element"],this._provdmterms["relation"]);
	
	this._provattributes = {"used" : {"provattr1":"prov:activity","provattr2":"prov:entity"},
				            "actedOnBehalfOf" : {"provattr1":"prov:subordinate","provattr2":"prov:responsible"},
				            "alternateOf" : {"provattr1":"prov:subject","provattr2":"prov:alternate"},
				            "hasAnnotation" : {"provattr1":"prov:record","provattr2":"prov:note"},
				            "specializationOf" : {"provattr1":"prov:subject","provattr2":"prov:specialization"},
				            "wasAssociatedWith" : {"provattr1":"prov:activity","provattr2":"prov:agent"},
				            "wasDerivedFrom" : {"provattr1":"prov:generatedentity","provattr2":"prov:usedentity"},
				            "wasEndedBy" : {"provattr1":"prov:activity","provattr2":"prov:agent"},
				            "wasGeneratedBy" : {"provattr1":"prov:entity","provattr2":"prov:activity"},
				            "wasStartedBy" : {"provattr1":"prov:activity","provattr2":"prov:agent"},
				            "tracedTo" : {"provattr1":"prov:entity","provattr2":"prov:ancestor"},
				            "wasInformedBy" : {"provattr1":"prov:informed","provattr2":"prov:informant"},
				            "wasRevisionOf" : {"provattr1":"prov:newer","provattr2":"prov:older"},
				            "wasAttributedTo" : {"provattr1":"prov:entity","provattr2":"prov:agent"},
				            "wasQuotedFrom" : {"provattr1":"prov:quote","provattr2":"prov:quoted"},
				            "wasSummaryOf" : {"provattr1":"prov:summarizedEntity","provattr2":"prov:fullEntity"},
				            "hadOriginalSource" : {"provattr1":"prov:entity","provattr2":"prov:source"}}

	this.getNamespaceDict = getNamespaceDict;
	this.processJSON = processJSON;
	this.resolveQname = resolveQname;
	this.processValue = processValue;

	this.nsdict = this.getNamespaceDict();
	this.container = this.processJSON();
	
	this._parseQueryArgument = _parseQueryArgument;
	this._matchRelation = _matchRelation;
	this._queryByType = _queryByType;
	this._limitByIdentifier = _limitByIdentifier;
	this._limitByCstrRlat = _limitByCstrRlat;
	this._limitByCstrAttr = _limitByCstrAttr;
	this._queryContainer = _queryContainer;
	this.q = provjsQuery;
}

function getNamespaceDict(){
	var nsdict = {};
	for (var key in this.json)
		if (key=="prefix")
			for (var prefix in this.json.prefix)
				nsdict[prefix] = this.json.prefix[prefix];
	return nsdict;	
}

function processJSON(){
	var container = {};
	var testmsg = "";
	this._visitedrecord = [];
	for (var key in this.json) {
		if (key=="account"){
			for (var account in this.json.account){
//				if ($.inArray(account,this._visitedrecord) == -1)
				if(this._visitedrecord.hasItem(account)==false)
				{
					this._visitedrecord.push(account);
					var accURI = this.resolveQname(account);
					if(typeof container["account"] == "undefined")
						container["account"]={};
					var accJSON = new provjs(this.json.account[account]);
					accJSON.nsdict = this.nsdict;
					accJSON.container = accJSON.processJSON();
					container["account"][accURI] = accJSON.container;
					container["account"][accURI]["RESERVED_provjstype"] = "account";
				}
			}
		}
		else if (key!="prefix"){
			if(this._provdmterms["record"].indexOf(key) >= 0){
				for (var record in this.json[key]){
					var recordURI = this.resolveQname(record);
					if(typeof container[key] == "undefined")container[key]={};
					container[key][recordURI]={};
					container[key][recordURI]["RESERVED_provjstype"] = key;
					for (var attr in this.json[key][record]){
						var attrURI = this.resolveQname(attr);
						// test code
						testmsg = testmsg + "<br>"+ this.json[key][record][attr].toString() + isPROVArray(this.json[key][record][attr]).toString();
						// end test code
						var value = this.processValue(this.json[key][record][attr]);
						//test code
						testmsg = testmsg + "<br>&nbsp;&nbsp;&nbsp;&nbsp;"+ value.toString();
						$('#test').html(testmsg);
						//end test code
						container[key][recordURI][attrURI]=value;
					}
				}
			}
			else{
				var attrURI = this.resolveQname(key);
				var value = this.processValue(this.json[key]);
				container[attrURI] = value;
			}
		}
	}
	return container;
}

function resolveQname(qname){
	var URI = qname;
	// currently not resolving qname
	return URI;
}

function processValue(value){
	//TODO: This needs thoughts... currently difficult to express PROVLiteral in query.
	var rt = [];
	if (isPROVArray(value)){
		if (value[1]=="prov:array"){
			for(var i=0;i<value[0].length;i++){
				rt.push(this.processValue(value[0][i])[0]);
			}
		}
		else{
			rt.push(new PROVLiteral(value[0],value[1]));
		}
	}
	else{
		rt.push(value);
	}
	return rt;
}

function isPROVArray(obj){
	var rt = false;
		if (obj.constructor.toString().indexOf("Array") != -1)
			if(obj.length == 2)
					rt = true;
	return rt;
}

function _parseQueryArgument(argument){
	var parseresult = { "identifier" : null,
						"type" : null,
						"account" : null,
						"cstrrlat" : [],
						"cstrattr" : []};

//    var arglist = argument.split("<<");
	var arglist = argument;
	for(var i=0;i<arglist.length;i++){
		arglist[i]=arglist[i].removeSurroundingSpace();		
	}
	for(var i=0;i<arglist.length;i++){
		if(arglist[i].indexOf(">>") >= 0){
			var cstr = arglist[i].split(">>");
			for(var j=0;j<cstr.length;j++){
				cstr[j]=cstr[j].removeSurroundingSpace();		
			}
			if (cstr.length == 3){
				var cstrrlat = {};
				for(var z=0;z<3;z++)
					if(cstr[z].startsWith("#"))
						cstr[z] = cstr[z].substring(1);
				cstrrlat["provattr1"] = cstr[0];
				cstrrlat["relation"]=cstr[1];
				cstrrlat["provattr2"]=cstr[2];
				parseresult["cstrrlat"].push(cstrrlat);
			}
			else if (cstr.length == 2){
				var cstrattr = {};
				cstrattr["attribute"] = cstr[0];
				cstrattr["value"]=cstr[1];
				parseresult["cstrattr"].push(cstrattr);
			}
		}
		else if(arglist[i].startsWith("account#")){
			parseresult["account"]=arglist[i].split("account#")[1];
		}
		else if(arglist[i].startsWith("#")){
			parseresult["identifier"]=arglist[i].substring(1);
		}
		else{
			parseresult["type"] = arglist[i];
		}
	}
	return parseresult;
}

function provjsQuery(argument){
	var rtlist = [];
	if(typeof argument == "undefined")argument=null;
	if(argument==null) alert("No parametre given for query");
	else {// parse argument here.
		querypara = this._parseQueryArgument(argument);
		rtlist = this._queryContainer(querypara);
	}
	return rtlist;
}

function _queryContainer(querypara){
	var rtlist = [];
	this._queriedrecord = [];
	if(querypara.type==null || querypara.type=="record")
		for(var i=0;i<this._provdmterms["record"].length;i++)
			rtlist = rtlist.concat(this._queryByType(this.container,this._provdmterms["record"][i],querypara.account));
	else if(querypara.type == "relation")
		for(var i=0;i<this._provdmterms["relation"].length;i++)
			rtlist = rtlist.concat(this._queryByType(this.container,this._provdmterms["relation"][i],querypara.account));
	else if(querypara.type == "element"){
		for(var i=0;i<this._provdmterms["element"].length;i++){
			rtlist = rtlist.concat(this._queryByType(this.container,this._provdmterms["element"][i],querypara.account));
			}
		}
	else
		rtlist = this._queryByType(this.container,querypara.type,querypara.account);
	
	if(querypara.identifier!=null)
		rtlist = this._limitByIdentifier(rtlist,querypara.identifier);

	for (var i=0;i<querypara.cstrrlat.length;i++){
		rtlist = this._limitByCstrRlat(rtlist,querypara.cstrrlat[i],querypara.account);
	}
		
	for (var j=0;j<querypara.cstrattr.length;j++){
		rtlist = this._limitByCstrAttr(rtlist,querypara.cstrattr[j]);
	}
	return rtlist;
}

function _queryByType(container,type,account){
	var rtlist = [];
	test = container;
	if(account==null){
		if(typeof container[type] != "undefined"){
			for(var id in container[type]){
				var item = {};
				item[id] = container[type][id];
				rtlist.push(item);
			}
		}
		if(typeof container["account"] != "undefined")
			for(var acc in container["account"])
				rtlist = rtlist.concat(this._queryByType(container["account"][acc],type,null));
	}
	else{
		if(typeof container["account"] != "undefined"){
			for(var acc in container["account"]){
				if(acc==account){
					rtlist = rtlist.concat(this._queryByType(container["account"][acc],type,null));
				}
				else{
					rtlist = rtlist.concat(this._queryByType(container["account"][acc],type,account));
				}
			}
		}
	}
	return rtlist;
}

function _limitByIdentifier(candidates,identifier){
	var rtlist = [];
	for(var i=0;i<candidates.length;i++){
		for(var id in candidates[i]){
			if(id==identifier){
				rtlist.push(candidates[i]);
			}
		}
	}
	return rtlist;
}

function _limitByCstrRlat(candidates,cstrrlat,account){
	var rtlist = [];
	var rlatlist = [];
	for(var i=0;i<this._provdmterms["relation"].length;i++)
		rlatlist = rlatlist.concat(this._queryByType(this.container,this._provdmterms["relation"][i],account));

	for(var i=0;i<candidates.length;i++){
		for(var id in candidates[i]){
			var cstr = {};
			var insertposition = -1;
			for(var key in cstrrlat){
				if (cstrrlat[key]=="$"){
					cstr[key]=id;
					insertposition = key;
				}
				else
					cstr[key]=cstrrlat[key];
			}
			if(insertposition != -1){
				if(insertposition!="relation"){
					if(this._provdmterms["relation"].hasItem(cstr["relation"])){
						var matchlist = this._queryByType(this.container,cstr["relation"],account)
						matchlist = this._limitByCstrAttr(matchlist, {"attribute":this._provattributes[cstr["relation"]].provattr1,"value":cstr["provattr1"]});
						matchlist = this._limitByCstrAttr(matchlist, {"attribute":this._provattributes[cstr["relation"]].provattr2,"value":cstr["provattr2"]});
						if (matchlist.length>0) rtlist.push(candidates[i]);
					}
					else if(cstr["relation"]=="**"){
						for(var r=0;r<this._provdmterms["relation"].length;r++){
							cstr["relation"] = this._provdmterms["relation"][r];
							var matchlist = this._limitByCstrAttr(rlatlist, {"attribute":this._provattributes[cstr["relation"]].provattr1,"value":cstr["provattr1"]});
							matchlist = this._limitByCstrAttr(matchlist, {"attribute":this._provattributes[cstr["relation"]].provattr2,"value":cstr["provattr2"]});
							if (matchlist.length>0) rtlist.push(candidates[i]);
						}
					}
				}
				else {
					var rlatname = candidates[i][id].RESERVED_provjstype;
					if((candidates[i][id][this._provattributes[rlatname].provattr1] == cstr["provattr1"])||(cstr["provattr1"]=="**"))
						if((candidates[i][id][this._provattributes[rlatname].provattr2] == cstr["provattr2"])||(cstr["provattr2"]=="**"))
							rtlist.push(candidates[i]);
//					var matchlist = this._limitByIdentifier(rlatlist,cstr[1])
//					matchlist = this._limitByCstrAttr(matchlist, {"attribute":this._provattributes[cstr[1]].provattr1,"value":cstr[0]});
//					matchlist = this._limitByCstrAttr(matchlist, {"attribute":this._provattributes[cstr[1]].provattr2,"value":cstr[2]});
//					if (matchlist.length>0) rtlist.push(candidates[i]);
				}
			}
		}
	}
	return rtlist;
}

function _limitByCstrAttr(candidates,cstrattr){
	var rtlist = [];
	for(var i=0;i<candidates.length;i++){
		for(var id in candidates[i]){
			for (var attr in candidates[i][id]){
				if(attr == cstrattr.attribute){
					for(var k=0;k<candidates[i][id][attr].length;k++){
						if(candidates[i][id][attr][k]== cstrattr.value){
							rtlist.push(candidates[i]);
						}
						else if(cstrattr.value == "**"){
							rtlist.push(candidates[i]);
						}
					}
				}
			}
		}
	}
	return rtlist;
}

function _matchRelation(cstr,relation){
	
}

String.prototype.startsWith = function (str){
	return this.indexOf(str) == 0;
};

String.prototype.removeSurroundingSpace = function (){
	var rt = this.substring(0);
	if (rt.startsWith(" ")) {
		rt=rt.substring(1);
	}
	else if (rt[rt.length-1]==" ") {
		rt=rt.substring(0,(rt.length)-1);
	}
	if (rt.startsWith(" ") || (rt[rt.length-1]==" "))	{
		rt=rt.removeSurroundingSpace();
	}
	return rt;
};

Array.prototype.hasItem = function (item){
	var rt = false;
	for(var i=0;i<this.length;i++)
		if (this[i]==item)
			rt = true;
	return rt;
};
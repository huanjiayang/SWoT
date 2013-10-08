// Simple helper function creates a new element from a name, so you don't have to add the brackets etc.
$.createElement = function(name)
{
    return $('<'+name+' />');
};

// JQ plugin appends a new element created from 'name' to each matched element.
$.fn.appendNewElement = function(name)
{
    this.each(function(i)
    {
        $(this).append('<'+name+' />');
    });
    return this;
}

/* xml root element - because html() does not include the root element and we want to 
 * include <report /> in the output. There may be a better way to do this.
 */
var $root = $('<XMLDocument />');

$root.append
(
    // one method of adding a basic structure
    $('<Request />').append
    (
        $('<submitter />').append
        (
            $('<name />').text('client')
        )
    )
    // example of our plugin
    .appendNewElement('queries')
);

// get a reference to Request
var $Request = $root.find('Request');

// get a reference to queries
var $queries = $Request.find('queries');
// or find queries from the $root like this: $root.find('Request>queries');

// create 'Alice'
var $newData = $.createElement('data');
// add 'name' element using standard jQuery
$newData.append($.createElement('key').text('SEID'));
// add 'grade' element using our helper
$newData.append($.createElement('value').text('*'));

// add 'SEID' to <queries />
$queries.append($newData);

// display the markup as text
alert($root.html());
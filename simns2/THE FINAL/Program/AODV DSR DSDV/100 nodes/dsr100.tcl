
set val(chan)		Channel/WirelessChannel ;# channel type
set val(prop) 		Propagation/TwoRayGround ;# radio-propagation model
set val(netif)		Phy/WirelessPhy ;
set val(mac)		Mac/802_11 ;
set val(ifq)		CMUPriQueue
set val(ll)		LL ;
set val(ant)		Antenna/OmniAntenna ;
set val(ifqlen)		50 ;
set val(nn)		100 ;
set val(x)              400
set val(y)              400
set val(rp)		DSR;# routing protocol
set val(cp)		"cbr100" ;
set val(sc)		"scence100" ;

set ns_ [new Simulator]
set tracefd [open dsr100.tr w]
$ns_ trace-all $tracefd

set namtracefd [open aodv.nam w]
$ns_ namtrace-all-wireless $namtracefd $val(x) $val(y)

set topo [new Topography]
$topo load_flatgrid $val(x) $val(y)

set god_ [new God]
create-god $val(nn)

set chan_1_ [new $val(chan)]

$ns_ node-config -adhocRouting $val(rp) \
		-llType $val(ll) \
		-macType $val(mac) \
		-ifqType $val(ifq) \
		-ifqLen $val(ifqlen) \
		-antType $val(ant) \
		-propType $val(prop) \
		-phyType $val(netif) \
		          -channel $chan_1_  \
		-topoInstance $topo \
		-agentTrace ON \
		-routerTrace ON \
		-macTrace OFF \
		-movementTrace OFF \

for {set i 0} {$i < $val(nn)} {incr i} {
set node_($i) [$ns_ node]
$node_($i) random-motion 0
}

source $val(cp)
source $val(sc)

for {set i 0} {$i < $val(nn)} {incr i} {
$ns_ at 100.1 "$node_($i) reset";
}
$ns_ at 100.2 "stop"
$ns_ at 100.3 "puts \"NS exiting...­\"; $ns_ halt"
proc stop {} {
global ns_ tracefd namtracefd
$ns_ flush-trace

close $tracefd
close $namtracefd

#exec nam aodv.nam &
exit 0
}
$ns_ run

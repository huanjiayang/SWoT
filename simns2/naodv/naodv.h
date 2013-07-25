#ifndef __aodv_h__
#define __aodv_h__

//#include "protoname_pkt.h"
//#include <agent.h>
//#include <packet.h>
//#include <trace.h>
//#include <timer-handler.h>
//#include <random.h>

#include <cmu-trace.h>
#include <priqueue.h>
#include <aodv/aodv_rtable.h>
#include <aodv/aodv_rqueue.h>
#include <classifier/classifier-port.h>


#define CURRENT_TIME Scheduler::instance().clock()
#define JITTER (Random::uniform()*0.5)

class naodv; // forward declaration

/* Timers  (Broadcast ID, Hello, Neighbor Cache, Route Cache) */

class BroadcastTimer : public Handler {
public:
	BroadcastTimer(AODV* a) : agent(a) {}
	void	handle(Event*);
private:
 AODV   *agent;
 Event	intr;
};


/* Agent */

class naodv : public Agent {

	/* Friends */
	friend class Protoname_PktTimer;

	/* Private members */
	nsaddr_t ra_addr_;
	protoname_state state_;
	protoname_rtable rtable_;
	int accesible_var_;
	u_int8_t seq_num_;

protected:

	PortClassifier* dmux_; // For passing packets up to agents.
	Trace* logtarget_; // For logging.
	Protoname_PktTimer pkt_timer_; // Timer for sending packets.

	inline nsaddr_t& ra_addr() { return ra_addr_; }
	inline protoname_state& state() { return state_; }
	inline int& accessible_var() { return accessible_var_; }

	void forward_data(Packet*);
	void recv_protoname_pkt(Packet*);
	void send_protoname_pkt();

	void reset_protoname_pkt_timer();

public:

	naodv(nsaddr_t);
	int command(int, const char*const*);
	void recv(Packet*, Handler*);

};

#endif
	
	
	
	
	
	
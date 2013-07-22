int protoname_pkt::offset_;
static class ProtonameHeaderClass : public PacketHeaderClass {
public:
	ProtonameHeaderClass() : PacketHeaderClass("PacketHeader/naodv",
		sizeof(hdr_protoname_pkt)) {
		bind_offset(&hdr_protoname_pkt::offset_);
	}
} class_rtProtoProtoname_hdr;

/* Timer */
static class ProtonameClass : public TclClass {
public:
	ProtonameClass() : TclClass("Agent/naodv") {}
	TclObject* create(int argc, const char*const* argv) {
		assert(argc == 5);
		return (new naodv((nsaddr_t)Address::instance().str2addr(argv[4])));
	}
} class_rtProtoProtoname;


/* Agent */
void
Protoname_PktTimer::expire(Event* e) {
agent_->send_protoname_pkt();
agent_->reset_protoname_pkt_timer();
}

int
Protoname::command(int argc, const char*const* argv) {
	(argc == 2) {
		if (strcasecmp(argv[1], "start") == 0) {
			pkt_timer_.resched(0.0);
			return TCL_OK;
		}
		else if (strcasecmp(argv[1], "print_rtable") == 0) {
			if (logtarget_ != 0) {
				sprintf(logtarget_->pt_->buffer(), "P %f _%d_ Routing Table",
					CURRENT_TIME,
					ra_addr());
				logtarget_->pt_->dump();
				rtable_.print(logtarget_);
			}
			else {
				fprintf(stdout, "%f _%d_ If you want to print this routing table "
					"you must create a trace file in your tcl script",
					CURRENT_TIME,
					ra_addr());
			}
			return TCL_OK;
		}
	}
	else if (argc == 3) {
		// Obtains corresponding dmux to carry packets to upper layers
		if (strcmp(argv[1], "port-dmux") == 0) {
			dmux_ = (PortClassifier*)TclObject::lookup(argv[2]);
			if (dmux_ == 0) {
				fprintf(stderr, "%s: %s lookup of %s failed\n",
					__FILE__,
					argv[1],
					argv[2]);
				return TCL_ERROR;
			}
			return TCL_OK;
		}
		// Obtains corresponding tracer
		else if (strcmp(argv[1], "log-target") == 0 ||
			strcmp(argv[1], "tracetarget") == 0) {
			logtarget_ = (Trace*)TclObject::lookup(argv[2]);
			if (logtarget_ == 0)
				return TCL_ERROR;
			return TCL_OK;
		}
	}
	// Pass the command to the base class
	return Agent::command(argc, argv);
}
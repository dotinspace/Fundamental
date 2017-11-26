#include "MrLumps.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	plugin->slug = "MrLumps";
	//plugin->name = "MrLumps";
	plugin->website = "https://github.com/MrLumps";
	p->addModel(createModel<SEQEuclidWidget>(plugin->slug, "SEQE", "SEQ-Euclid",SEQUENCER_TAG));
   p->addModel(createModel<VCS1x8Widget>(plugin->slug, "VCS", "1x8 Voltage Controlled Switch",SWITCH_TAG));
   p->addModel(createModel<VCS2x4Widget>(plugin->slug, "VCS", "2x4 Voltage Controlled Switch",SWITCH_TAG));
}

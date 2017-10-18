#include "MrLumps.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	plugin->slug = "MrLumps";
	plugin->name = "MrLumps";
	plugin->homepageUrl = "https://github.com/MrLumps";
	createModel<SEQEuclidWidget>(plugin, "SEQE", "SEQ-Euclid");
   createModel<VCS1x8Widget>(plugin, "VCS", "1x8 Voltage Controlled Switch");
   createModel<VCS2x4Widget>(plugin, "VCS", "2x4 Voltage Controlled Switch");
}

#include "VersionControl.h"
#include "CinderFileSys.h"
#include "Ember.h"

namespace Ember {
	static Cinder::CFSFile backlog;
	const std::string COMP_ERROR = "Error-Please download latest version at https://github.com/strah19/Ember\n";

	void InitBacklog() {
		backlog = Cinder::CFSFile("backlog.log");

		if (backlog.IsFileEmpty()) {
			backlog.Write(EMBER_VERSION);
			backlog.Write("\n");
		}

		backlog.Write("Entry[");
		backlog.Write(__TIME__);
		backlog.Write(" :: ");
		backlog.Write(__DATE__);
		backlog.Write("]:\n");
		backlog.Write("Version: ");
		backlog.Write(EMBER_VERSION);
		backlog.Write("\n");
	}

	void AddBacklogMessage(const std::string& msg) {
		backlog.Write(msg);
	}

	void CompatibilityCheck() {
		if (EMBER_VERSION != std::stof(backlog.ReadLine(0))) {
			backlog.ResetReadLocation();
			AddBacklogMessage(COMP_ERROR);
			std::cout << COMP_ERROR << std::endl;
			
			exit(EXIT_FAILURE);
		}
	}
}
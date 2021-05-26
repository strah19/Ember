#ifndef VERSION_CONTROL_H
#define VERSION_CONTROL_H

#include <string>

namespace Ember {
	void InitBacklog();
	void AddBacklogMessage(const std::string& msg);

	void CompatibilityCheck();
}

#endif // !VERSION_CONTROL_H

#ifndef EVENT_STACK_H
#define EVENT_STACK_H

#include <string>
#include <vector>
#include <functional>

namespace Ember {
	class EventLayer {
	public:
		EventLayer(const std::string& name) : layer_name(name) { }
		std::string Name() { return layer_name; }
	private:
		std::string layer_name;
	};

	class EventStack {
	public:
		enum class EventStackError {
			LayerNameCouldNotBeFound, LayerDeletionFailed, DuplicitLayerFound, Null
		};

		EventStack(const std::string& first_name);
		EventStackError SetCurrentLayer(const std::string& name);
		EventStackError AddLayer(const std::string& name);
		EventStackError DeleteLayer(const std::string& name);
		EventStackError DrawEventToLayer(const std::function<void()>& event_func, const std::string& name);

		std::string GetLayer() { return event_layers[current_layer].Name(); }
	private:
		size_t IndexLayers(const std::string& name);

		size_t current_layer;
		std::vector<EventLayer> event_layers;
	};
}

#endif // !EVENT_STACK_H

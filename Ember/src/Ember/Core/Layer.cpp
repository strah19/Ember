#include "Layer.h"

namespace Ember {
	EventStack::EventStack(const std::string& first_name) {
		AddLayer(first_name);
		current_layer = 0;
	}

	EventStack::EventStackError EventStack::SetCurrentLayer(const std::string& name) {
		size_t index = IndexLayers(name);
		if (index != -1) {
			current_layer = index;
			return EventStackError::Null;
		}

		return EventStackError::LayerNameCouldNotBeFound;
	}

	EventStack::EventStackError EventStack::AddLayer(const std::string& name) {
		size_t index = IndexLayers(name);
		if (index == -1) {
			event_layers.push_back(name);
			return EventStackError::Null;
		}
		return EventStackError::DuplicitLayerFound;
	}

	EventStack::EventStackError EventStack::DeleteLayer(const std::string& name) {
		size_t index = IndexLayers(name);
		if (index != -1) {
			event_layers.erase(event_layers.begin() + index);
			return EventStackError::Null;
		}

		return EventStackError::LayerDeletionFailed;
	}

	EventStack::EventStackError EventStack::DrawEventToLayer(const std::function<void()>& event_func, const std::string& name) {
		if (name == event_layers[current_layer].Name()) {
			event_func();
			return EventStackError::Null;
		}
		return EventStackError::LayerNameCouldNotBeFound;
	}

	size_t EventStack::IndexLayers(const std::string& name) {
		for (size_t i = 0; i < event_layers.size(); i++) {
			if (event_layers[i].Name() == name) {
				return i;
			}
		}

		return -1;
	}

}
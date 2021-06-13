#include "Layer.h"

namespace Ember {
	LayerStack::~LayerStack() {
		for (Layer* layer : layers) {
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		layers.emplace(layers.begin() + insert_index, layer);
		insert_index++;
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(layers.begin(), layers.begin() + insert_index, layer);
		if (it != layers.begin() + insert_index) {
			layer->OnDetach();
			layers.erase(it);
			insert_index--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(layers.begin() + insert_index, layers.end(), overlay);
		if (it != layers.end()) {
			overlay->OnDetach();
			layers.erase(it);
		}
	}
}
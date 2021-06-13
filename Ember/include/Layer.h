#ifndef LAYER_H
#define LAYER_H

#include <string>
#include "Events.h"

namespace Ember {
	class Layer {
	public:
		Layer(const std::string& name)
			: name(name) { }
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float delta) {}
		virtual void UserDefEvent(Event& event) {}
		virtual void UpdateGui() { }

		inline std::string GetName() const { return name; }
	protected:
		std::string name;
	};

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline std::vector<Layer*>::iterator begin() { return layers.begin(); }
		inline std::vector<Layer*>::iterator end() { return layers.end(); }
		inline std::vector<Layer*>::reverse_iterator rbegin() { return layers.rbegin(); }
		inline std::vector<Layer*>::reverse_iterator rend() { return layers.rend(); }

		inline std::vector<Layer*>::const_iterator begin() const { return layers.begin(); }
		inline std::vector<Layer*>::const_iterator end() const { return layers.end(); }
		inline std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers.rbegin(); }
		inline std::vector<Layer*>::const_reverse_iterator rend() const { return layers.rend(); }
	private:
		std::vector<Layer*> layers;
		unsigned int insert_index = 0;
	};
}

#endif // !LAYER_H

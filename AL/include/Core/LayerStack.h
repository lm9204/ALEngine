#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "Core/Base.h"
#include "Core/Layer.h"

#include <vector>

namespace ale
{
class LayerStack
{
  public:
	LayerStack() = default;
	~LayerStack();

	void pushLayer(Layer *layer);
	void pushOverlay(Layer *overlay);
	void popLayer(Layer *layer);
	void popOverlay(Layer *overlay);
	void onDetach();

	std::vector<Layer *>::iterator begin()
	{
		return m_Layers.begin();
	}
	std::vector<Layer *>::iterator end()
	{
		return m_Layers.end();
	}
	std::vector<Layer *>::reverse_iterator rbegin()
	{
		return m_Layers.rbegin();
	}
	std::vector<Layer *>::reverse_iterator rend()
	{
		return m_Layers.rend();
	}

	std::vector<Layer *>::const_iterator begin() const
	{
		return m_Layers.begin();
	}
	std::vector<Layer *>::const_iterator end() const
	{
		return m_Layers.end();
	}
	std::vector<Layer *>::const_reverse_iterator rbegin() const
	{
		return m_Layers.rbegin();
	}
	std::vector<Layer *>::const_reverse_iterator rend() const
	{
		return m_Layers.rend();
	}

  private:
	std::vector<Layer *> m_Layers;
	uint32_t m_LayerInsertIndex = 0;
};

} // namespace ale

#endif
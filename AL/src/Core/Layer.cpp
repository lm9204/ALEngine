#include "Core/Layer.h"
#include "ALpch.h"

namespace ale
{

Layer::Layer(const std::string &debugName) : m_DebugName(debugName)
{
}

void Layer::onAttach()
{
}

void Layer::onDetach()
{
}

void Layer::onUpdate(Timestep ts)
{
}

void Layer::onImGuiRender()
{
}

void Layer::onEvent(Event &event)
{
}

} // namespace ale
#include "StdAfx.h"
#include "GraphNode.h"

#include <algorithm>

GraphNode::GraphNode(void):GraphBase()
{
}
GraphNode::~GraphNode(void)
{
	for(unsigned int i=0;i<m_ChildNodes.size();++i)
	{
		SAFE_DELETE (m_ChildNodes.at(i));
	}
}
void GraphNode::AddChild(GraphBase *NewGraphElement)
{
	NewGraphElement->SetParent(this);
	m_ChildNodes.push_back(NewGraphElement);
}

void GraphNode::RemoveChild(GraphBase *toRemove){
	toRemove->SetParent(NULL);
	vector<GraphBase*>::iterator foundElement;
	foundElement = std::find(m_ChildNodes.begin(),m_ChildNodes.end(),toRemove);
	if ( foundElement != m_ChildNodes.end())
		m_ChildNodes.erase(foundElement);
}

void GraphNode::OnResize()
{
	//call Tick on every child
	std::vector<GraphBase*>::iterator  it;
	for(it=m_ChildNodes.begin();it<m_ChildNodes.end();++it)
	{
		(*it)->OnResize();
	}
}

void GraphNode::Tick(float dTime)
{
	//call Tick on every child
	std::vector<GraphBase*>::iterator  it;
	for(it=m_ChildNodes.begin();it<m_ChildNodes.end();++it)
	{
		(*it)->Tick(dTime);
	}
}
void GraphNode::CalculateFinalWorldMatrix()
{
	//calc world matrix in base class
	//GraphBase::CalculateFinalWorldMatrix();

	//call CalculateFinalWorldMatrix on every child
	std::vector<GraphBase*>::iterator  it;
	for(it=m_ChildNodes.begin();it<m_ChildNodes.end();++it)
	{
		(*it)->CalculateFinalWorldMatrix();
	}
}
void GraphNode::Draw() const
{
	//call Draw on every child
	std::vector<GraphBase*>::const_iterator  it;
	for(it=m_ChildNodes.begin();it<m_ChildNodes.end();++it)
	{
		(*it)->Draw();
	}
}
void GraphNode::Draw(const ID3D10EffectTechnique* pTechnique) const
{
	//call Draw on every child
	std::vector<GraphBase*>::const_iterator  it;
	for(it=m_ChildNodes.begin();it<m_ChildNodes.end();++it)
	{
		(*it)->Draw(pTechnique);
	}
}
void GraphNode::KeyPressed(WPARAM cKey)
{
	//call KeyPressed on every child
	std::vector<GraphBase*>::iterator  it;
	for(it=m_ChildNodes.begin();it<m_ChildNodes.end();++it)
	{
		(*it)->KeyPressed(cKey);
	}
}
void GraphNode::SetPhysXGlobalPose()
{
	//call SetPhysXGlobalPose on every child
	std::vector<GraphBase*>::iterator  it;
	for(it=m_ChildNodes.begin();it<m_ChildNodes.end();++it)
	{
		(*it)->SetPhysXGlobalPose();
	}
}
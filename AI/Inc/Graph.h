#ifndef INCLUDED_AI_GRAPH_H
#define INCLUDED_AI_GRAPH_H

#include "Common.h"

namespace AI {

struct Node
{
	std::vector<size_t> neighbors;
};

constexpr auto InvalidNode = std::numeric_limits<size_t>::max();

template <typename NodeType>
class Graph
{
public:
	void Initialized(size_t nodeCount);

	void Cleanup();

	size_t GetNodeCount() const;

	NodeType& GetNode(size_t index);
	const NodeType& GetNode(size_t index) const;

private:
	std::vector<NodeType> mNodes;
};

template <typename NodeType>
inline void Graph<NodeType>::Initialized(size_t nodeCount)
{
	mNodes.resize(nodeCount);
}

template<typename NodeType>
inline void Graph<NodeType>::Cleanup()
{
	mNodes.clear();
}

template<typename NodeType>
inline size_t Graph<NodeType>::GetNodeCount() const
{
	return mNodes.size();
}

template<typename NodeType>
inline NodeType& Graph<NodeType>::GetNode(size_t index)
{
	return mNodes[index];
}

template<typename NodeType>
inline const NodeType& Graph<NodeType>::GetNode(size_t index) const
{
	return mNodes[index];
}


} // namespace AI

#endif // !INCLUDED_AI_GRAPH_H
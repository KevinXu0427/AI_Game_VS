#ifndef INCLUDED_GRAPHSEARCH_H
#define INCLUDED_GRAPHSEARCH_H

#include"Graph.h"

namespace AI::GraphSearch {

struct Context
{
	int count = 0;
	int insertIndex = 0;

	size_t nodeCount;
	std::list<size_t> openList;
	std::list<size_t> closedList;
	std::vector<size_t> parent;
	std::vector<bool> open;
	std::vector<bool> closed;

	std::vector<float> g;
	std::vector<float> f;


	size_t start = InvalidNode;
	size_t end = InvalidNode;
	bool found = false;
	
	void Init(size_t nodes)
	{
		nodeCount = nodes;
	}

	void Reset()
	{
		openList.clear();
		closedList.clear();
		parent.clear();
		open.clear();
		closed.clear();
		g.clear();
		f.clear();

		g.resize(nodeCount, 0.0f);
		f.resize(nodeCount, 0.0f);
		parent.resize(nodeCount, InvalidNode);
		open.resize(nodeCount, false);
		closed.resize(nodeCount, false);

		found = false;
	}
};

template<typename GraphType, typename OpenPolicy, typename NextPolicy, typename ValidationPolicy>
struct BaseSearch
{
	BaseSearch(const GraphType& graph)
		: graph(graph)
	{}

	void Execute(Context& context)
	{
		if (!context.open[context.start])
		{
			Open(context, context.start, AI::InvalidNode);
		}
		while (!context.found && !context.openList.empty())
		{
			size_t nodeIndex = Next(context);

			if (nodeIndex == context.end)
			{
				context.found = true;
			}
			else
			{
				auto& node = graph.GetNode(nodeIndex);
				for (auto& n : node.neighbors)
				{
					/*if (!context.open[n])
					{
						Open(context, n, nodeIndex);
					}*/

					if (IsValid(context, n))
					{
						Open(context, n, nodeIndex);
					}

				}
			}

			context.closedList.push_back(nodeIndex);
			context.closed[nodeIndex] = true;

		}
	}
	const GraphType& graph;
	OpenPolicy Open;
	NextPolicy Next;
	ValidationPolicy IsValid;
};

struct PushBackPolicy
{
	void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
	{
		context.openList.push_back(nodeIndex);
		context.open[nodeIndex] = true;
		context.parent[nodeIndex] = parentIndex;
	}
};

struct PopFrontPolicy
{
	size_t operator()(Context& context)
	{
		auto next = context.openList.front();
		context.openList.pop_front();
		return next;
	}
};

struct PopBackPolicy
{
	size_t operator()(Context& context)
	{
		auto next = context.openList.back();
		context.openList.pop_back();
		return next;
	}
};

template <typename CostPolicy>
struct InsertSortedPolicy
{
	void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
	{
		float g = Cost(context, parentIndex, nodeIndex);

		if (!context.open[nodeIndex])
		{
			auto it = context.openList.begin();
			for (; it != context.openList.end(); ++it)
			{
				//bigger add front
				if (g > context.g[*it])
					break;  
			}

			context.openList.insert(it, nodeIndex);
			context.open[nodeIndex] = true;
			context.g[nodeIndex] = g;
			context.parent[nodeIndex] = parentIndex;
		}
		else if (context.g[nodeIndex] > g)
		{  
			auto it = context.openList.begin();
			for (; it != context.openList.end(); ++it)
			{
				if (*it == nodeIndex)
				{
					context.openList.remove(*it);
					auto i = context.openList.begin();
					for (; i != context.openList.end(); ++i)
					{
						//bigger add front
						if (g > context.g[*i])
							break;
					}
					context.openList.insert(i, nodeIndex);
					break;
				}
			}
		
			context.g[nodeIndex] = g;
			context.parent[nodeIndex] = parentIndex;
		}
	}

	CostPolicy Cost;

};

template <typename CostPolicyG, typename CostPolicyH>
struct InsertSortedPolicyAstar
{
	void operator()(Context& context, size_t nodeIndex, size_t parentIndex)
	{
		float g = CostG(context, parentIndex, nodeIndex);
		float h = CostH(context, parentIndex, nodeIndex);
		float f = g + h;
		if (!context.open[nodeIndex])
		{
			auto it = context.openList.begin();
			for (; it != context.openList.end(); ++it)
			{
				//bigger add front
				if (f > context.f[*it])
					break;
			}

			
			context.openList.insert(it, nodeIndex);
			context.open[nodeIndex] = true;
			context.g[nodeIndex] = g;
			context.f[nodeIndex] = f;

			context.parent[nodeIndex] = parentIndex;
			
			
		}
		else if (context.f[nodeIndex] > f)
		{
			auto it = context.openList.begin();
			for (; it != context.openList.end(); ++it)
			{
				if (*it == nodeIndex)
				{
					context.openList.remove(*it);
					// need to check the sort again
					auto i = context.openList.begin();
					for (; i != context.openList.end(); ++i)
					{
						//bigger add front
						if (f > context.f[*i])
							break;
					}
					context.openList.insert(i, nodeIndex);
					break;
				}
			}

			context.g[nodeIndex] = g;
			context.f[nodeIndex] = f;

			context.parent[nodeIndex] = parentIndex;
		}
	}

	CostPolicyG CostG;
	CostPolicyH CostH;
};


template <typename GraphType, typename ValidationPolicy>
using BFS = BaseSearch<GraphType, PushBackPolicy, PopFrontPolicy, ValidationPolicy>;

template <typename GraphType, typename ValidationPolicy>
using DFS = BaseSearch<GraphType, PushBackPolicy, PopBackPolicy, ValidationPolicy>;

template <typename GraphType, typename ValidationPolicy, typename CostPolicy >
using Dijkstra = BaseSearch<GraphType, InsertSortedPolicy<CostPolicy>, PopBackPolicy, ValidationPolicy>;

template <typename GraphType, typename ValidationPolicy, typename CostPolicyG, typename CostPolicyH >
using AStart = BaseSearch<GraphType, InsertSortedPolicyAstar<CostPolicyG, CostPolicyH >, PopBackPolicy, ValidationPolicy>;

}// namespace AI::GraphSearch

#endif // !INCLUDED_GRAPHSEARCH_H
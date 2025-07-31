export module directed_graph;

import directed_graph_node;

import <vector>;
import <algorithm>;

export template <typename T>
class directed_graph
{
public:
	//insert method
	bool insert(const T& node_value);
	bool insert(T&& node_value);

	bool erase(const T& node_value);

	bool insert_edge(const T& from_node_value, const T& to_node_value);

	bool erase_edge(const T& from_node_value, const T& to_node_value);

	void clear() noexcept;

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	bool operator==(const directed_graph& rhs) const;
	bool operator!=(const directed_graph& rhs) const;

	void swap(directed_graph& other_graph) noexcept;

	[[nodiscard]] size_t size() const noexcept;

	[[nodiscard]] std::set<T> get_adjacent_nodes_values(const T& node_value) const;
private:
	using nodes_container_type = std::vector<details::graph_node<T>>;
	nodes_container_type m_nodes;

	typename nodes_container_type::iterator findNode(const T& node_value);
	typename nodes_container_type::const_iterator findNode(const T& node_value) const;

	size_t get_index_of_node(const typename nodes_container_type::const_iterator& node) const noexcept;

	void remove_all_links_to(typename nodes_container_type::const_iterator node_iter);
};

template<typename T>
typename directed_graph<T>::nodes_container_type::iterator directed_graph<T>::findNode(const T& node_value)
{
	return std::find_if(std::begin(m_nodes), std::end(m_nodes), [&node_value](const auto& node) {return node.value() == node_value; });
}

template<typename T>
typename directed_graph<T>::nodes_container_type::const_iterator directed_graph<T>::findNode(const T& node_value) const
{
	return const_cast<directed_graph<T>*>(this)->findNode(node_value);
}


template<typename T>
size_t directed_graph<T>::get_index_of_node(const typename nodes_container_type::const_iterator& node) const noexcept
{
	const auto index{ std::distance(std::cbegin(m_nodes), node) };
	return static_cast<size_t>(index);
}

template<typename T>
void directed_graph<T>::remove_all_links_to(typename nodes_container_type::const_iterator node_iter)
{
	const size_t node_index{ get_index_of_node(node_iter) };

	for (auto&& node : m_nodes) {
		auto& adjacencyIndices{ node.get_adjacent_nodes_indices() };
		adjacencyIndices.erase(node_index);
		std::vector<size_t> indices(std::begin(adjacencyIndices), std::end(adjacencyIndices));
		std::for_each(std::begin(indices),std::end(indices)), 
			[node_index](size_t& index) {
				if (index > node_index) { --index };
			});
		adjacencyIndices.clear();
		adjacencyIndices.insert(std::begin(indices), std::end(indices));
	}
}

template<typename T>
bool directed_graph<T>::insert(const T& node_value)
{
	
	T copy{ node_value };
	return insert(std::move(copy));
}

template<typename T>
bool directed_graph<T>::insert(T&& node_value)
{
	auto iter{ findNode(node_value) };
	if (iter != std::end(m_nodes))
	{
		return false;
	}
	m_nodes.emplace_back(this, std::move(node_value));

	return true;
}

template<typename T>
bool directed_graph<T>::erase(const T& node_value)
{
	auto iter{ findNode(node_value) };
	if (iter == std::end(m_nodes)) {
		return false;
	}
	remove_all_links_to(iter);
	m_nodes.erase(iter);
	return true;
}

template<typename T>
bool directed_graph<T>::insert_edge(const T& from_node_value, const T& to_node_value)
{
	const auto from{ findNode(from_node_value) };
	const auto to{ findNode(to_node_value) };
	if (from == std::end(m_nodes) || to == std::end(m_nodes)) {
		return false;
	}
	cons size_t to_index{ get_index_of_node(to) };
	return from->get_adjacent_nodes_indices().insert(to_index).second;
}

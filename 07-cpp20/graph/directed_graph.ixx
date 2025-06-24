export module directed_graph;

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

};
#ifndef LIST_H
#define LIST_H

namespace NS {
	struct Node	{
		Node(const int value, Node* last = nullptr, Node* next = nullptr) : m_value(value), m_last(last), m_next(next) {}
		const int m_value;
		Node* m_last;
		Node* m_next;
	};

	class List {
	public:
		List() : m_begin(nullptr), m_end(nullptr) {}
		~List() {
			while (m_end != nullptr)
			{
				erase(m_end);
			}
		}
		Node* push_back(const int value) {
			Node* new_node(new Node(value));
			if (m_begin != nullptr) {
				new_node->m_last = m_end;
				m_end->m_next = new_node;
				m_end = new_node;
			}
			else {
				m_begin = new_node;
				m_end = new_node;
			}
			return new_node;
		}
		void erase(Node* Node) {
			if (Node != m_begin && Node != m_end) {
				Node->m_last->m_next = Node->m_next;
				Node->m_next->m_last = Node->m_last;
			}
			else if (Node == m_begin && Node != m_end) {
				Node->m_next->m_last = nullptr;
				m_begin = Node->m_next;
			}
			else if (Node == m_end && Node != m_begin) {
				Node->m_last->m_next = nullptr;
				m_end = Node->m_last;
			}
			else {
				m_begin = nullptr;
				m_end = nullptr;
			}
			delete Node;
		}
		Node* begin() { return m_begin; }
		Node* end() { return m_end; }
		const Node* begin() const { return m_begin; }
		const Node* end() const { return m_end; }
		bool empty() const { return m_begin == nullptr; }
	private:
		Node * m_begin;
		Node* m_end;
	};
}

#endif // !LIST_H


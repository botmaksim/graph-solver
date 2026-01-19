#ifndef TEMPLATESTACK_H
#define TEMPLATESTACK_H

#include <stdexcept>
#include <cstddef>

/**
 * @brief A custom template Stack implementation using a Singly Linked List.
 * Represents the "Custom Data Structure" requirement.
 * Manages memory manually via Nodes.
 */
template <typename T>
class TemplateStack {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& val, Node* n = nullptr) : data(val), next(n) {}
    };

    Node* head;
    size_t count;

public:
    /** @brief Constructor. */
    TemplateStack() : head(nullptr), count(0) {}

    /** @brief Destructor. Clears all nodes to prevent memory leaks. */
    ~TemplateStack() {
        clear();
    }

    /** @brief Pushes a value onto the stack. */
    void push(const T& value) {
        head = new Node(value, head);
        count++;
    }

    /** @brief Pops the top value. Throws if empty. */
    void pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
    }

    /** @brief Returns reference to top value. Throws if empty. */
    T& top() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return head->data;
    }

    /** @brief Checks if stack is empty. */
    bool isEmpty() const {
        return head == nullptr;
    }

    /** @brief Returns number of elements. */
    size_t size() const {
        return count;
    }

    /** @brief Clears the stack. */
    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }
    
    /** @brief Checks if stack contains a specific value (Linear search). */
    bool contains(const T& value) const {
        Node* current = head;
        while (current) {
            if (current->data == value) return true;
            current = current->next;
        }
        return false;
    }
};

#endif // TEMPLATESTACK_H
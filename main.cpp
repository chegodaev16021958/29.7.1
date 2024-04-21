#include <iostream>
#include <mutex>

struct Node
{
    int value;
    Node* next;
    std::mutex* node_mutex;
};

class FineGrainedQueue
{
private:
    Node* head;
    std::mutex* queue_mutex;

public:
    void insertIntoMiddle(int value, int pos)
    {
        queue_mutex->lock();

        Node* prev = this->head;

        if (this->head->next == nullptr)
        {
            Node* node = new Node;
            this->head->next = node;
            node->value = value;
            node->next = nullptr;
            queue_mutex->unlock();
            return;
        }

        queue_mutex->unlock();

        prev->node_mutex->lock();

        for (int i = 1; i < pos; ++i)
        {
            Node* old_prev = prev;
            prev = old_prev->next;
            old_prev->node_mutex->unlock();
            prev->node_mutex->lock();

            if (prev->next == nullptr)
            {
                break;
            }
        }

        Node* node = new Node;
        node->value = value;
        node->next = prev->next;
        prev->next = node;
        prev->node_mutex->unlock();

        return;
    }
};


int main()
{
    return 0;
}
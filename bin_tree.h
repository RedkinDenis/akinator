typedef char* data_t;

struct Node
{
    data_t data = 0;
    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;
};

err NodeInsert (Node* head, data_t num);

err treeKill (Node* head);

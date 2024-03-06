err NodeInsert (Node* head, data_t num)
{
    CHECK_PTR(head);

    Node* node_temp = {};
    void* temp = 0;

    CALLOC(node_temp, Node, 1);

    node_temp->data = num;

    while(1)
    {
        if (num >= head->data)
        {
            if (head->right == NULL)
            {
                head->right = node_temp;
                break;
            }
            else
                head = head->right;
        }
        else
        {
            if (head->left == NULL)
            {
                head->left = node_temp;
                break;
            }
            else
                head = head->left;
        }
    }

    return SUCCESS;
}

err treeSearch (Node* head, data_t srch, Node** return_t)
{
    CHECK_PTR(head);
    CHECK_PTR(return_t);

    while(1)
    {
        if (head->data == srch)
        {
            *return_t = head;
            return SUCCESS;
        }

        if (head->data > srch)
        {
            if (head->left != NULL)
            {
                return treeSearch(head->left, srch, return_t);
            }
            else
                return UNFOUND;
        }

        if (head->data < srch)
        {
            if (head->right != NULL)
            {
                return treeSearch(head->right, srch, return_t);
            }
            else
            {
                *return_t = NULL;
                return UNFOUND;
            }
        }
    }
}

struct DoubleNode
{  
    SelectorList *selectors;
    AttributeList *attributes;
    DoubleNode *next;
    DoubleNode *prev;
};

class DoubleLinkedList
{
public:
    DoubleNode *head;
    DoubleNode *ListOfSections[T];
  //  DoubleNode *next;
  //  DoubleNode *prev;

    // constructor to create an empty LinkedList
    DoubleLinkedList()
    {
        head = NULL;
        DoubleNode *temp;
        for (int i = 0; i < T; i++)
        {
            ListOfSections[i] = new DoubleNode[i];
        }
    }

    // Add new element at the end of the list
    void push_back(int newElement)
    {
        DoubleNode *newNode = new DoubleNode();
        newNode->selectors = new SelectorList();
        newNode->attributes = new AttributeList();
        newNode->next = NULL;
        newNode->prev = NULL;
        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            DoubleNode *temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newNode;
            newNode->prev = temp;
        }
    }

    void PrintList()
    {
        DoubleNode *temp = head;
        if (temp != NULL)
        {
            while (temp != NULL)
            {
                temp = temp->next;
            }
            cout << endl;
        }
        else
        {
            cout << "The list is empty.\n";
        }
    }

    ~DoubleLinkedList()
    {
        DoubleNode *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
            delete[] temp;
        }
    }
};
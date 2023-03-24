
struct double_linked_list
{
    int data;
    double_linked_list *next;
    double_linked_list *prev;
}typedef double_linked_list;

*double_linked_list new_double_linked_list(int data)
{
    double_linked_list *list = (double_linked_list *)malloc(sizeof(double_linked_list));
    list->data = data;
    list->next = nullptr;
    list->prev = nullptr;
    return list;
}

int main()
{
    return 0;
}
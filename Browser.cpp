#include "Browser.h"

Browser::Browser() {
    windows.append(Window());
}

void Browser::newWindow() 
{
    windows.prepend(Window());
}

void Browser::closeWindow() 
{
    if (windows.getFirstNode() != NULL) // if there is at least one window
        windows.removeNode(windows.getFirstNode());
}

void Browser::switchToWindow(int index) 
{
    windows.moveToIndex(index, 0);
}

Window &Browser::getWindow(int index) 
{
    return windows.getNodeAtIndex(index) -> data;
}

void Browser::moveTab(Window &from, Window &to) 
{
    to.newTab(from.getActiveTab());
    from.closeTab();
}

void Browser::mergeWindows(Window &window1, Window &window2) 
{
    window2.changeActiveTabTo(0);
    while (!window2.isEmpty())
       moveTab(window2, window1);
}

void Browser::mergeAllWindows() 
{
    if (!windows.isEmpty() && windows.getSize() != 1)
    {
        Node<Window> *first = windows.getFirstNode();
        Node<Window> *second = first->next;
        
        while (second != first)
        {
            mergeWindows(first->data, second->data);
            second = second->next;
        }
    }
}

void Browser::closeAllWindows() 
{
    windows.removeAllNodes();
}

void Browser::closeEmptyWindows() 
{
    Node<Window> *temp = windows.getFirstNode();
    
    int size = windows.getSize();
    
    for (int i = 0; i < size; i++)
    {
        if (temp->data.isEmpty())
        {
            temp = temp->next;
            windows.removeNode(temp->prev); 
        }
        else
            temp = temp->next;
    }
}

void Browser::print() {
    Node<Window> *head = windows.getFirstNode();
    if(head == NULL) {
        std::cout << "The browser is empty" << std::endl;
    } else {
        (head -> data).print();
        head = head -> next;
        while(head != windows.getFirstNode()) {
            (head -> data).print();
            head = head -> next;
        }
    }
}

#include "Window.h"

// activeTab = -1;

Window::Window() 
{
    this->activeTab = 0;
    tabs.append(Tab());
}

Tab Window::getActiveTab() {
    
    if (tabs.isEmpty())
        return Tab();
    else
        return tabs.getNodeAtIndex(activeTab)->data;
}

bool Window::isEmpty() const 
{
    return tabs.isEmpty() || activeTab == -1;
}

void Window::newTab(const Tab &tab) 
{
    tabs.insertAtIndex(tab, ++activeTab);    
}

void Window::closeTab()
{
    if (!tabs.isEmpty())
    {
        tabs.removeNodeAtIndex(activeTab);
        
        if (tabs.getFirstNode() == NULL)
            activeTab = -1;
        
        else if (activeTab == tabs.getSize())
            activeTab--;
    }
}

void Window::moveActiveTabTo(int index) 
{
    tabs.moveToIndex(activeTab, index);
    if (index >= tabs.getSize())
        activeTab = tabs.getSize() - 1;
    else
        activeTab = index;
}

void Window::changeActiveTabTo(int index) 
{
    if (index < tabs.getSize() && index >= 0)
        activeTab = index;
}

void Window::addTab(Node<Tab> &tab)
{
    if (tabs.getFirstNode() == NULL)
        activeTab = 0;
    tabs.append(tab.data);
}

void Window::print() {
    if(tabs.isEmpty()) std::cout << "Window is empty" << std::endl;
    else std::cout << "The active tab is tab " << activeTab << std::endl;
    tabs.print();
}

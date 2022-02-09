//Andrea Iriarte
//Project 1
//June 4, 2020


#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define CAPACITY 80

struct Node //doubly linked list
{
    Node* prev;
    Node* next;
    string line;
    Node() : line(0), next(nullptr), prev(nullptr) {} //default
    Node (string data) : line(data), next(nullptr), prev(nullptr) {} //initialize string data
};
    
class LineEditor
{
    private:
        Node* head;
        Node* tail;
        unsigned int lineCount;
    public:
        LineEditor(); // default constructor
        ~LineEditor(); //destructor
        LineEditor &operator=(const LineEditor &document); // copy constructor
        
        //inserting
        void InsertEnd(string line);
        void InsertFront(string line);
        void InsertAt(unsigned int index, string line);
        
        //other functionality
        void Edit (unsigned int index, string data);
        void Search (string line);
        void PrintAll();
        
        //deleting 
        void DeleteLine (unsigned int n);
        void DeleteHead();
        void DeleteTail();
        
        //some helper functions
        Node* GetNode(unsigned int n);
        void InsertHelper (unsigned int index, string line);
        unsigned int GetLineCount();



};

LineEditor::LineEditor()
{
    head = nullptr;
    tail = nullptr;
    lineCount = 0;
}

LineEditor& LineEditor::operator=(const LineEditor &document)
{
    /*if (lineCount > 0)
    {
        Clear();
    }*/

    //shallow copy
    this->head = nullptr;
    this->tail = nullptr;
    this->lineCount = 0;

    //deep copy
    Node* temp = document.head;
    for (unsigned int x = 0; x < document.lineCount; x++)
    {
        InsertEnd(temp->line);
        temp = temp->next;
    }

    return *this;
}

LineEditor::~LineEditor()
{
    //if list is not empty, cycle through and delete every node
    
    if (lineCount > 0)
    {
        Node* temp = head->next;

        while (temp != nullptr)
        {
            Node* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }
}

void LineEditor::InsertEnd(string line)
{
    //if the list is empty, then the line becomes the head of the list
    //else, it becomes the tail
    
    if (lineCount == 0) 
     {
         InsertFront(line);
     }
     
     else
     {
         Node* insert = new Node(line);
         
         tail->next = insert;
         insert->prev = tail;
         tail = insert;
         
         lineCount++;
     }
     
     
}

void LineEditor::InsertFront(string line)
{
    //if this is the first line in the doc, 
    //then both head and tail should point to it initially
    Node* insert = new Node(line);
    
    if (lineCount == 0) 
     {
         head = insert;
         tail = insert;
         
         head->next = tail;
         tail->prev = head;
         
     }
     
     else
     {
         head->prev = insert;
         insert->next = head;
         head = insert;
     }
     
     lineCount++;
}

void LineEditor::InsertAt(unsigned int index, string line)
{
    //if the desired index would place the line at either th front or the end
    //those functions are called
    //if the index is out of bounds, it is ignored
    //if the index is in between, then the program cycles through the nodes (GetNode)
    
    if (index == (lineCount + 1))
    {
        InsertEnd(line);
    }
    
    else if (index == 1)
    {
        InsertFront(line);
    }
    
    else if (index <= lineCount)
    {
        Node* temp = GetNode(index);
        
        Node* insert = new Node(line);
        
        insert->prev = temp->prev;
        insert->next = temp;
        insert->prev->next= insert;
        temp->prev = insert;
        
        lineCount++;
    }
}

void LineEditor::Edit(unsigned int index, string data)
{
    if (index >= 1 && index <= lineCount)
    {
        Node* temp = GetNode(index);

        if (data.length() > CAPACITY)
        {
            temp->line = data.substr(0, CAPACITY);
            index++;
            data = data.substr(CAPACITY);
            
            InsertHelper(index, data);
        }
        
        else
        {
            temp->line = data;
        }
    }
}

void LineEditor::Search(string line)
{
    string toCompare = "";
    Node* slow = head;
    bool isFound = false;
    
    unsigned int LC = 1;
    
    unsigned int lineNumber = line.length() / 80 + 1;
    
    while (LC <= lineCount - lineNumber + 1)
    {
        Node* fast = slow;
        
        for (unsigned int i = 0; i < lineNumber; i++)
        {
            toCompare += fast->line;
            fast = fast->next;
        }
        
        if (toCompare.find(line) != std::string::npos)
        {
            isFound = true;
            
            fast = slow;
            
            unsigned int LC2 = LC;

            for (unsigned int i = 0; i < lineNumber; i++)
            {
                cout << LC2 << " " << fast->line << "\n";
                LC2++;
                fast = fast->next;
            }
            
        }
        
        LC++;
        slow = slow->next;
        toCompare = "";
    }
    
    if (!isFound)
    {
        cout << "not found\n";
    }
}

void LineEditor::PrintAll()
{
    //cycle through and print all
    
    unsigned int LC = 1;
    
    Node* temp = head;
    
    while (LC <= lineCount)
    {
        cout << LC << " " << temp->line << "\n";
        temp = temp->next;
        LC++;
    }
}

void LineEditor::DeleteLine(unsigned int n)
{
    //ignore if index is out of bounds
    //calls delete head if index = 1 and delete tail if index = lineCount
    //cycles through otherwise
    
    if (lineCount > 0)
    {
        if (n > lineCount)
            return;
        
        if (n == 1)
        {
            DeleteHead();
        }
        
        else if (n < lineCount)
        {

            Node* temp = GetNode(n);
            
            
            Node* after = temp->next;
            Node* before = temp->prev;
        
            temp->next = nullptr;
            temp->prev = nullptr;
        
            after->prev = before;
            before->next = after;
        
            delete temp; //O(n)
        
        }
        
        else if (n == lineCount)
        {
            DeleteTail();
        }
    
        lineCount--;
    }
}

void LineEditor::DeleteHead()
{
    Node* temp = head;
    
    if (lineCount > 1)
    {
        
        head = temp->next;
    }
    
    delete temp; //O(1) in all cases
    

}

void LineEditor::DeleteTail()
{
    Node* temp = tail;
    tail = temp->prev;
    delete temp; // O(1) in all cases
    
}

Node* LineEditor::GetNode(unsigned int n)
{
    Node* temp = head;
    unsigned int LC = 1;
    
    while (LC < n)
    {
        temp = temp->next;
        LC++;
        
    }
    
    return temp;
}

//calls InsertAt 80 chars at a time
void LineEditor::InsertHelper(unsigned int index, string line)
{
    if (line.length() > CAPACITY)
    {
        while (line.length() > CAPACITY)
        {
            InsertAt(index, line.substr(0, CAPACITY));
            index++;
            line = line.substr(CAPACITY);
        }
        
        InsertAt(index, line);
    }
    
    else
    {
        InsertAt(index, line);
    }
}

unsigned int LineEditor::GetLineCount()
{
    return lineCount;
}

int main()
{
    string input = "";
    LineEditor document;
    
    while (input != "quit")
    {
        getline(cin, input);
        
        if (input == "quit")
        {
            break;
        }
        
        else if (input.substr(0, 9) == "insertEnd")
        {
           string data = input.substr(11, (input.length() - 12));
           
           unsigned int index = document.GetLineCount() + 1;
           document.InsertHelper(index, data);
        }
        
        else if (input.substr(0, 6) == "insert")
        {
            size_t pos = input.find("\"");
            int length = input.length() - 8 - input.substr(pos).length();
            unsigned int index = stoi(input.substr(7, length));
            string data = input.substr(pos + 1, (input.length() - 10 - length));
            
            document.InsertHelper(index, data);
        }
        
        else if (input.substr(0,4) == "edit")
        {
            size_t pos = input.find("\"");
            int length = input.length() - 6 - input.substr(pos).length();
            unsigned int index = stoi(input.substr(5, length));
            string data = input.substr(pos + 1, (input.length() - length - 8));
            
            document.Edit(index, data);
        }
        
        else if (input.substr(0, 6) == "search")
        {
            document.Search(input.substr(8, input.length() - 9));
        }
        
        else if (input == "print")
        {
            document.PrintAll();
        }
        
        else if (input.substr(0,6) == "delete")
        {
            unsigned int n = stoi(input.substr(7));
            document.DeleteLine(n);
        }
    }
}

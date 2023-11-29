#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <string>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <set>
#include <chrono>
#include "dataStructures.h"

using namespace std;
using json=nlohmann::json;

struct Person
{
    string id;
    string first_name;
    string last_name;
    string email;
    string children_first_name;
    string children_last_name;
    string children_email;

    Person* prev; //for LL
    Person* next; //for DL

};

struct PersonBT
{
    string id;
    string first_name;
    string last_name;
    string email;
    string children_first_name;
    string children_last_name;
    string children_email;

    PersonBT* left;
    PersonBT* right;

    PersonBT(const string& _id, const string& _first_name, const string& _last_name, const string& _email,
           const string& _children_first_name, const string& _children_last_name, const string& _children_email)
        : id(_id), first_name(_first_name), last_name(_last_name), email(_email),
          children_first_name(_children_first_name), children_last_name(_children_last_name), children_email(_children_email),
          left(nullptr), right(nullptr)
    {}
};


struct SearchResult
{
    Person *person;
    chrono::nanoseconds duration;
};

struct SearchResultForVector
{
    Person person;
    chrono::nanoseconds duration;
};


array<Person, 10000> getDataAsArray(const json& data) 
{
    array<Person, 10000> people; // Using 10000 elements in JSON file

    for (size_t i=0; i<data.size(); i++) 
    {
        const auto& item=data[i];
        Person& person=people[i];
        person.id=item.value("_id","not found");
        person.first_name=item.value("first_name","not found");
        person.last_name=item.value("last_name","not found");
        person.email=item.value("email","not found");

        if (item.find("children") !=item.end()) 
        {
            const json& children=item["children"];
            person.children_first_name=children.value("first_name", "not found");
            person.children_last_name=children.value("last_name", "not found");
            person.children_email=children.value("email", "not found");
        } else 
        {
            person.children_first_name="NULL";
            person.children_last_name="NULL";
            person.children_email="NULL";
        }
    }

    return people;
}

SearchResult linearSearchArray(const array<Person, 10000>& people, const string& id) 
{
    auto start = chrono::high_resolution_clock::now();

    for (size_t i = 0; i < people.size(); ++i) 
    {
        if (people[i].id == id) 
        {
            auto stop = chrono::high_resolution_clock::now();
            return {const_cast<Person*>(&people[i]), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
    }

    auto stop = chrono::high_resolution_clock::now();
    
    return {nullptr, chrono::duration_cast<chrono::nanoseconds>(stop - start)};
}


void sortArray(array<Person, 10000>& people)
{
    //sorting by ID
    sort(people.begin(), people.end(), [](const Person& a, const Person& b)
    {
        return a.id < b.id;
    });
}

SearchResult binarySearchArray(array<Person, 10000>& people, const string& id)
{

    auto start = chrono::steady_clock::now();

    int left = 0;
    int right = people.size() - 1;

    while (left <= right)
    {
        int middle = left + (right - left) / 2;
        const Person& person = people[middle];

        if (person.id == id)
        {
            auto stop = chrono::steady_clock::now();
            return {const_cast<Person*>(&person), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
        else if (person.id < id)
        {
            left = middle + 1;
        }   
        else
        {
            right = middle - 1;
        }
    }

    return {nullptr, chrono::nanoseconds(-1)};
}

vector<Person> getDataAsVector(const json& data) 
{
    vector<Person> people;

    for (const auto& item : data) 
    {
        Person person;
        person.id=item.value("_id","not found");
        person.first_name=item.value("first_name","not found");
        person.last_name=item.value("last_name","not found");
        person.email=item.value("email","not found");

        if (item.find("children") !=item.end()) 
        {
            const json& children=item["children"];
            person.children_first_name=children.value("first_name","not found");
            person.children_last_name=children.value("last_name","not found");
            person.children_email=children.value("email","not found");
        } 
        else 
        {
            person.children_first_name="NULL";
            person.children_last_name="NULL";
            person.children_email="NULL";
        }

        people.push_back(person);
    }

    return people;
}

SearchResultForVector linearSearchVector(const vector<Person>& people, const string& searchId) 
{
    auto start = chrono::high_resolution_clock::now();

    for (const Person& person : people) 
    {
        if (person.id == searchId) 
        {
            auto stop = chrono::high_resolution_clock::now();
            return {person, chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
    }

    Person notFoundPerson; // "not found" person if no such ID
    notFoundPerson.id = "not found";
    return {notFoundPerson, chrono::nanoseconds(-1)};
}

void sortVector(vector<Person>& peopleVector) 
{
    // Lambda function to compare people by their ID
    sort(peopleVector.begin(), peopleVector.end(), [](const Person& a, const Person& b) {
        return a.id < b.id;
    });
}

SearchResultForVector binarySearchVector(const vector<Person>& peopleVector, const string& targetId) 
{
    vector<Person> tempPeople = peopleVector; // copy to not modify the original vector
    sortVector(tempPeople);

    auto start = chrono::high_resolution_clock::now();

    int left = 0;
    int right = tempPeople.size() - 1;

    while (left <= right) 
    {
        int mid = left + (right - left) / 2;
        if (tempPeople[mid].id == targetId) 
        {
            auto stop = chrono::high_resolution_clock::now();
            return {tempPeople[mid], chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        } 
        else if (tempPeople[mid].id < targetId) 
        {
            left = mid + 1; // search right half
        } 
        else 
        {
            right = mid - 1; // search left half
        }
    }

    Person notFoundPerson; // "not found" person if no such ID
    notFoundPerson.id = "not found";
    return {notFoundPerson, chrono::nanoseconds(-1)};
}


list<Person> insertDataIntoLinkedList(const json& data) 
{
    list<Person> people;

    for (const auto& item : data) 
    {
        Person person;

        person.id = item.value("_id", "not found");
        person.first_name = item.value("first_name", "not found");
        person.last_name = item.value("last_name", "not found");
        person.email = item.value("email", "not found");

        if (item.find("children") != item.end()) 
        {
            const json& children = item["children"];
            person.children_first_name = children.value("first_name", "not found");
            person.children_last_name = children.value("last_name", "not found");
            person.children_email = children.value("email", "not found");
        } 
        else
        {
            person.children_first_name = "NULL";
            person.children_last_name = "NULL";
            person.children_email = "NULL";
        }

        people.push_back(person);
    }

    return people;
}



bool compareByID(const Person& a, const Person& b) 
{
    return a.id < b.id;
}

SearchResult linearSearchLinkedList(const list<Person>& peopleList, const string& targetID) 
{
    auto start = chrono::high_resolution_clock::now();

    for (const auto& person : peopleList) 
    {
        if (person.id == targetID) 
        {
            auto stop = chrono::high_resolution_clock::now();
            return {const_cast<Person*>(&person), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
    }

    return {nullptr, chrono::nanoseconds(-1)};
}

SearchResult binarySearchLinkedList(const list<Person>& peopleList, const string& targetID) 
{
    // Sorted copy of the LL
    list<Person> sortedList = peopleList;
    sortedList.sort(compareByID);

    auto start = chrono::high_resolution_clock::now();

    for (const auto& person : sortedList) 
    {
        if (person.id == targetID) 
        {
            auto stop = chrono::high_resolution_clock::now();
            return {const_cast<Person*>(&person), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
    }

    return {nullptr, chrono::nanoseconds(-1)};
}


list<Person> insertDataIntoDoublyLinkedList(const json& data) 
{
    list<Person> people;

    for (const auto& item : data) 
    {
        Person person;
        person.id = item.value("_id", "not found");
        person.first_name = item.value("first_name", "not found");
        person.last_name = item.value("last_name", "not found");
        person.email = item.value("email", "not found");

        if (item.find("children") != item.end()) 
        {
            const json& children = item["children"];
            person.children_first_name = children.value("first_name", "not found");
            person.children_last_name = children.value("last_name", "not found");
            person.children_email = children.value("email", "not found");
        } 
        else 
        {
            person.children_first_name = "NULL";
            person.children_last_name = "NULL";
            person.children_email = "NULL";
        }

        people.push_back(person);
    }

    return people;
}

SearchResult linearSearchDoublyLinkedList(const list<Person>& peopleList, const string& targetID) 
{
    auto start = chrono::high_resolution_clock::now();

    for (const Person& person : peopleList) {
        if (person.id == targetID) 
        {
            auto stop = chrono::high_resolution_clock::now();
            return {const_cast<Person*>(&person), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
    }

    return {nullptr, chrono::nanoseconds(-1)}; 
}


SearchResult binarySearchDoublyLinkedList(const list<Person>& peopleList, const string& targetID) 
{
    
    list<Person> sortedList = peopleList;  // copy of the DLL, as usual
    sortedList.sort(compareByID);

    auto start = chrono::high_resolution_clock::now();

    for (const Person& person : sortedList) 
    {
        if (person.id == targetID) 
        {
            auto stop = chrono::high_resolution_clock::now();
            return {const_cast<Person*>(&person), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
    }

    return {nullptr, chrono::nanoseconds(-1)};
}


void pushDataToStack(const json& data, stack<Person>& peopleStack) 
{
    for (const auto& item : data) 
    {
        Person person;
        person.id=item.value("_id","not found");
        person.first_name=item.value("first_name","not found");
        person.last_name=item.value("last_name","not found");
        person.email=item.value("email","not found");

        if (item.find("children") !=item.end()) 
        {
            const json& children=item["children"];
            person.children_first_name=children.value("first_name","not found");
            person.children_last_name=children.value("last_name","not found");
            person.children_email=children.value("email","not found");
        } else 
        {
            person.children_first_name="NULL";
            person.children_last_name="NULL";
            person.children_email="NULL";
        }

        peopleStack.push(person);
    }
}

SearchResult linearSearchStack(const stack<Person> &peopleStack, const string &searchID)
{
    stack<Person> tempStack = peopleStack;

    auto start = chrono::high_resolution_clock::now();
    while (!tempStack.empty())
    {
        if (tempStack.top().id == searchID)
        {
            auto stop = chrono::high_resolution_clock::now();
            return {&tempStack.top(), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
        tempStack.pop();
    }

    return {nullptr, chrono::nanoseconds(-1)};
}

queue<Person> getDataAsQueue(const json& data) 
{
    queue<Person> peopleQueue;

    for (const auto& item : data) 
    {
        Person person;
        person.id=item.value("_id","not found");
        person.first_name=item.value("first_name","not found");
        person.last_name=item.value("last_name","not found");
        person.email=item.value("email","not found");

        if (item.find("children")!=item.end()) 
        {
            const json& children=item["children"];
            person.children_first_name=children.value("first_name","not found");
            person.children_last_name=children.value("last_name","not found");
            person.children_email=children.value("email","not found");
        } else 
        {
            person.children_first_name="NULL";
            person.children_last_name="NULL";
            person.children_email="NULL";
        }

        peopleQueue.push(person);
    }

    return peopleQueue;
}

SearchResult linearSearchQueue(const queue<Person>& peopleQueue, const string& target) 
{
    queue<Person> tempQueue = peopleQueue; // Copy of queue, as always

    auto start = chrono::high_resolution_clock::now();
    while (!tempQueue.empty()) 
    {
        if (tempQueue.front().id == target) 
        {
            auto stop = chrono::high_resolution_clock::now();
            return {&(tempQueue.front()), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
        tempQueue.pop();
    }

    return {nullptr, chrono::nanoseconds(-1)};
}

class BinaryTree
{
public:
    BinaryTree() : root(nullptr) {}

    void insertNode(PersonBT* person)
    {
        root = insert(root, person);
    }

    PersonBT* searchNode(const string& searchId) const
    {
        return search(root, searchId);
    }

    PersonBT* BFS(const string& searchId) const
    {
        return BFS(root, searchId);
    }

    PersonBT* DFS(const string& searchId) const
    {
        return DFS(root, searchId);
    }

    ~BinaryTree()
    {
        destroyTree(root);
    }

private:
    PersonBT* root;

    PersonBT* insert(PersonBT* node, PersonBT* person)
    {
        if (node == nullptr)
        {
            return person;
        }

        if (person->id < node->id)
        {
            node->left = insert(node->left, person);
        }
        else if (person->id > node->id)
        {
            node->right = insert(node->right, person);
        }

        return node;
    }

    PersonBT* search(PersonBT* node, const string& searchId) const
    {
        if (node == nullptr || node->id == searchId)
        {
            return node;
        }

        if (searchId < node->id)
        {
            return search(node->left, searchId);
        }
        else
        {
            return search(node->right, searchId);
        }
    }

    PersonBT* BFS(PersonBT* root, const string& searchId) const
    {
        queue<PersonBT*> q;
        q.push(root);

        while (!q.empty())
        {
            PersonBT* current = q.front();
            q.pop();

            if (current != nullptr)
            {
                if (current->id == searchId)
                {
                    return current;
                }

                q.push(current->left);
                q.push(current->right);
            }
        }

        return nullptr;
    }

    PersonBT* DFS(PersonBT* root, const string& searchId) const
    {
        stack<PersonBT*> s;
        s.push(root);

        while (!s.empty())
        {
            PersonBT* current = s.top();
            s.pop();

            if (current != nullptr)
            {
                if (current->id == searchId)
                {
                    return current;
                }

                s.push(current->right);
                s.push(current->left);
            }
        }

        return nullptr;
    }

    void destroyTree(PersonBT* node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};

unordered_map<string, Person> insertDataIntoUnorderedMap(const json& data) 
{
    unordered_map<string, Person> peopleMap;

    for (const auto& item : data) 
    {
        Person person;
        person.id = item.value("_id", "not found");
        person.first_name = item.value("first_name", "not found");
        person.last_name = item.value("last_name", "not found");
        person.email = item.value("email", "not found");

        if (item.find("children") != item.end()) 
        {
            const json& children = item["children"];
            person.children_first_name = children.value("first_name", "not found");
            person.children_last_name = children.value("last_name", "not found");
            person.children_email = children.value("email", "not found");
        } 
        else 
        {
            person.children_first_name = "NULL";
            person.children_last_name = "NULL";
            person.children_email = "NULL";
        }

        peopleMap[person.id] = person;
    }

    return peopleMap;
}

const Person* searchInUnorderedMap(const unordered_map<string, Person>& peopleMap, const string& searchID) 
{
    auto start = chrono::high_resolution_clock::now();
    auto it = peopleMap.find(searchID);
    if (it != peopleMap.end()) 
    {
        auto stop = chrono::high_resolution_clock::now();
        cout << "Time taken by HASH-SEARCH on UNORDERED MAP: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds" << endl;
        // Person found in the map
        return &(it->second);
    } 
    else 
    {
        // Person not found in the map
        return nullptr;
    }
}


int main() {
    ifstream f("MockData10000.json");
    json data = json::parse(f);
    f.close(); 

    string dataStructure;
    do
    {
        cout<<"Choose data structure: 'arr' - 'vec' - 'll' - 'dll' - 'sta' - 'que' - 'bt' - 'ht' - 'exit' to quit"<<endl;
        cin>>dataStructure;
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if( dataStructure == "arr")
        {
            cout<<"Using array:"<<endl;
            array<Person, 10000> peopleArray = getDataAsArray(data);
            string searchIdArray, arraySA;
            cout<<"Search algorithm: 'L' for Linear search - 'B' for Binary search "<<endl;
            cin>>arraySA;
            cout<<"Id of person to be found: "<<endl;
            cin>>searchIdArray;
            
            if(arraySA == "L")
            {
                SearchResult resultArray = linearSearchArray(peopleArray, searchIdArray);
                if (resultArray.person != nullptr)
                {
                    cout << "Person found by ID '" << searchIdArray << "' is " << resultArray.person->first_name << " " << resultArray.person->last_name << endl;
                    cout << "Time taken by LINEAR SEARCH on ARRAY: " << resultArray.duration.count() << " nanoseconds" << endl;
                }
                else 
                {
                    cout << "Person with ID '" << searchIdArray <<"' not found"<< endl;
                }
            }
            else if (arraySA == "B") 
            {
                sortArray(peopleArray);
                SearchResult resultArray = binarySearchArray(peopleArray, searchIdArray);
                if (resultArray.person != nullptr)
                {
                    cout << "Person found by ID '" << searchIdArray << "' is " << resultArray.person->first_name << " " << resultArray.person->last_name << endl;
                    cout << "Time taken by BINARY SEARCH on ARRAY: " << resultArray.duration.count() << " nanoseconds" << endl;
                }
                else
                {
                    cout << "Person with ID '" << searchIdArray << "' not found" << endl;
                }
            }
            else {cout<<"No such search algorithm: '"<< arraySA <<"'"<<endl;}
            
        }
        
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "vec")
        {
            vector<Person> peopleVector = getDataAsVector(data);
            cout<<"Using vector:"<<endl;
            string searchIdVector, VectorSA;
            cout<<"Search algorithm: 'L' for Linear search - 'B' for Binary search"<<endl;
            cin>> VectorSA;
            cout<<"Id of person to be found: "<<endl;
            cin>>searchIdVector;
            if(VectorSA == "L")
            {
                SearchResultForVector resultVector = linearSearchVector(peopleVector, searchIdVector);
                if (resultVector.person.id == "not found") 
                {
                    cout << "Person with ID '" << searchIdVector << "' not found" << endl;
                } 
                else 
                {
                    cout << "Person found by ID '" << searchIdVector << "' is " << resultVector.person.first_name << " " << resultVector.person.last_name << endl;
                    cout << "Time taken by LINEAR SEARCH on VECTOR: " << resultVector.duration.count() << " nanoseconds" << endl;
                }
            }
            else if (VectorSA == "B")
            {
                SearchResultForVector resultVector = binarySearchVector(peopleVector, searchIdVector);
                if (resultVector.person.id == "not found") 
                {
                    cout << "Person with ID '" << searchIdVector << "' not found" << endl;
                } 
                else 
                {
                    cout << "Person found by ID '" << searchIdVector << "' is " << resultVector.person.first_name << " " << resultVector.person.last_name << endl;
                    cout << "Time taken by BINARY SEARCH on VECTOR: " << resultVector.duration.count() << " nanoseconds" << endl;
                }   
            }
            else{ cout<<"No such search algorithm: '"<< VectorSA <<"'"<<endl;}
        }
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "ll")
        {
            list<Person> peopleLinkedList = insertDataIntoLinkedList(data);
            cout<<"Using linked list:"<<endl;
            string searchIdLL, LLSA;
            cout<<"Search algorithm: 'L' for Linear search - 'B' for Binary search"<<endl;
            cin>>LLSA;
            cout<<"Id of person to be found: "<<endl;
            cin>>searchIdLL;
            
            if(LLSA == "L")
            {
                SearchResult resultLL = linearSearchLinkedList(peopleLinkedList, searchIdLL);
                if (resultLL.person != nullptr) {
                    cout << "Person found by ID '" << searchIdLL << "' is " << resultLL.person->first_name << " " << resultLL.person->last_name << endl;
                    cout << "Time taken by LINEAR SEARCH on LINKED LIST: " << resultLL.duration.count() << " nanoseconds" << endl;
                }
                    else 
                {
                    cout << "Person with ID '"<< searchIdLL <<"' not found" << endl;
                }
            }
            else if (LLSA == "B")
            {
                SearchResult resultLL = binarySearchLinkedList(peopleLinkedList, searchIdLL);
                if (resultLL.person != nullptr) {
                    cout << "Person found by ID '" << searchIdLL << "' is " << resultLL.person->first_name << " " << resultLL.person->last_name << endl;
                    cout << "Time taken by BINARY SEARCH on LINKED LIST: " << resultLL.duration.count() << " nanoseconds" << endl;
                }  
                else 
                {
                    cout << "Person with ID '"<< searchIdLL <<"' not found" << endl;
                }
            }
            else{ cout<<"No such search algorithm: '"<< LLSA <<"'"<<endl;}
        }
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "dll")
        {
            list<Person> peopleDoublyLinkedList = insertDataIntoDoublyLinkedList(data);
            cout << "Using doubly linked list:" << endl;
            string searchIdDLL, DLLSA;
            cout<<"Search algorithm: 'L' for Linear search - 'B' for Binary search"<<endl;
            cin>>DLLSA;
            cout<<"Id of person to be found: "<<endl;
            cin>>searchIdDLL;
            if(DLLSA == "L")
            {
                SearchResult resultDLL = linearSearchDoublyLinkedList(peopleDoublyLinkedList, searchIdDLL);
                if (resultDLL.person != nullptr) {
                    cout << "Person found by ID '" << searchIdDLL << "' is " << resultDLL.person->first_name << " " << resultDLL.person->last_name << endl;
                    cout << "Time taken by LINEAR SEARCH on DOUBLY LINKED LIST: " << resultDLL.duration.count() << " nanoseconds" << endl;
                }
                else 
                {
                    cout << "Person with ID " << searchIdDLL << " not found" << endl;
                }
            }
            else if (DLLSA == "B")
            {
                SearchResult resultDLL = binarySearchDoublyLinkedList(peopleDoublyLinkedList, searchIdDLL);
                if (resultDLL.person != nullptr) {
                    cout << "Person found by ID '" << searchIdDLL << "' is " << resultDLL.person->first_name << " " << resultDLL.person->last_name << endl;
                    cout << "Time taken by BINARY SEARCH on DOUBLY LINKED LIST: " << resultDLL.duration.count() << " nanoseconds" << endl;
                }
                else 
                {
                    cout << "Person with ID " << searchIdDLL << " not found" << endl;
                }
            }
            else
            {
                cout<<"No such search algorithm '"<< DLLSA <<"'"<<endl;
            } 
        }
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "sta")
        {
            stack<Person> peopleStack;
            pushDataToStack(data, peopleStack);
            cout << "Using stack:" << endl;
            string searchIdStack, stackSA;
            cout<<"Search algorithm: 'L' for Linear search"<<endl;
            cin>>stackSA;
            cout<<"Id of person to be found: "<<endl;
            cin>>searchIdStack;
            if(stackSA == "L")
            {
                SearchResult resultStack = linearSearchStack(peopleStack, searchIdStack);
                if (resultStack.person != nullptr)
                {
                    cout << "Person found by ID '" << searchIdStack << "' is " << resultStack.person->first_name << " " << resultStack.person->last_name << endl;
                    cout << "Time taken by LINEAR SEARCH on ARRAY: " << resultStack.duration.count() << " nanoseconds" << endl;
                } 
                else 
                {
                    cout << "Person with ID '" << searchIdStack << "' not found" << endl;
                }
            }
            else
            {
                cout<<"No such search algorithm '"<< stackSA <<"'"<<endl;
            }
        }
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "que")
        {
            queue<Person> peopleQueue = getDataAsQueue(data);
            cout<<"Using queue: "<<endl;

            string searchIdQueue, queueSA;
            cout<<"Search algorithm: 'L' for Linear search"<<endl;
            cin>>queueSA;
            cout<<"Id of person to be found: "<<endl;
            cin>>searchIdQueue;
            
            if(queueSA == "L")
            {
                SearchResult resultQueue = linearSearchQueue(peopleQueue, searchIdQueue);
                if (resultQueue.person != nullptr)
                {
                    cout << "Person found by ID '" << searchIdQueue << "' is " << resultQueue.person->first_name << " " << resultQueue.person->last_name << endl;
                    cout << "Time taken by LINEAR SEARCH on QUEUE: " << resultQueue.duration.count() << " nanoseconds" << endl;
                }
                else 
                {
                    cout << "Person with ID '" << searchIdQueue <<"' not found" << endl;
                }
            }
            else
            {
                cout<<"No such search algorithm '"<< queueSA <<"'"<<endl;
            }
        }
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "bt")
        {
            BinaryTree binaryTree;

            for (const auto& person : data)
            {
                PersonBT* p = new PersonBT(
                    person.value("_id", "not found"),
                    person.value("first_name", "not found"),
                    person.value("last_name", "not found"),
                    person.value("email", "not found"),
                    person.value("children.first_name", "NULL"),
                    person.value("children.last_name", "NULL"),
                    person.value("children.email", "NULL")
                );

                binaryTree.insertNode(p);
            }

            string searchIdBT,BTSA;
            cout<<"Search algorithm: 'BS' for Binary search - 'BF' for Breadth first search - 'DF' for Deoth first search"<<endl;
            cin>>BTSA;
            cout << "Id of person to be found: " << endl;
            cin >> searchIdBT;

            if( BTSA == "BS")
            { 
                auto start = chrono::high_resolution_clock::now();
                PersonBT* foundPerson = binaryTree.searchNode(searchIdBT);
                auto stop = chrono::high_resolution_clock::now();

                if (foundPerson != nullptr)
                {
                    cout << "Person found by ID '" << searchIdBT << "' is " << foundPerson->first_name << " " << foundPerson->last_name << endl;
                }
                else
                {
                    cout << "Person with ID '" << searchIdBT << "' not found" << endl;
                }

                cout << "Time taken by BINARY SEARCH on BINARY TREE: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds" << endl;
            }
               
            else if (BTSA == "BF")
            {
                auto start = chrono::high_resolution_clock::now();
                PersonBT* foundPerson = binaryTree.BFS(searchIdBT);
                auto stop = chrono::high_resolution_clock::now();

                if (foundPerson != nullptr)
                {
                    cout << "Person found by ID '" << searchIdBT << "' using BFS is " << foundPerson->first_name << " " << foundPerson->last_name << endl;
                }
                else
                {
                    cout << "Person with ID '" << searchIdBT << "' not found using BFS" << endl;
                }

                cout << "Time taken by BREADTH-FIRST SEARCH on BINARY TREE: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds" << endl;
            }
            
            else if (BTSA == "DF")
            {
                auto start = chrono::high_resolution_clock::now();
                PersonBT* foundPerson = binaryTree.DFS(searchIdBT);
                auto stop = chrono::high_resolution_clock::now();

                if (foundPerson != nullptr)
                {
                    cout << "Person found by ID '" << searchIdBT << "' using DFS is " << foundPerson->first_name << " " << foundPerson->last_name << endl;
                }
                else
                {
                    cout << "Person with ID '" << searchIdBT << "' not found using DFS" << endl;
                }

                cout << "Time taken by DEPTH-FIRST SEARCH on BINARY TREE: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds" << endl;
                
            }

            else {cout<<"No such search algorithm '"<< BTSA <<"'"<<endl;}
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (dataStructure == "ht")
        {
            unordered_map<string, Person> peopleMap = insertDataIntoUnorderedMap(data);

            string searchIdHT,HTSA;
            cout<<"Search algorithm: 'HS' for Hash search"<<endl;
            cin>>HTSA;
            cout<<"Id of person to be found: "<<endl;
            cin>>searchIdHT;

            if (HTSA == "HS")
            {
                const Person* resultMap = searchInUnorderedMap(peopleMap, searchIdHT);
                if (resultMap) {
                    cout << "Person found by ID '" << searchIdHT << "' is " << resultMap->first_name << " " << resultMap->last_name << endl;
                } else {
                    cout << "Person with ID '" << searchIdHT << "' not found" << endl;
                }
            }
            else{cout<<"No such search algorithm '"<< HTSA <<"'"<<endl;}

        }
        else if (dataStructure == "exit")
        {
            break;
        }
        else 
        {
            cout<<"No such data structure "<<dataStructure<<endl;
        }
    }   
    while(dataStructure != "exit");
    
    return 0;
}

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

    Person* next; //for LL
    Person* prev; //for DLL
};


array<Person, 4> getDataAsArray(const json& data) 
{
    array<Person, 4> people; // Using 4 elements in JSON file

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

Person* linearSearchArray(const array<Person, 4>& people, const string& id) 
{
    for (const Person& person : people) 
    {
        if (person.id == id) {
            return const_cast<Person*>(&person); // return pointer to the person
        }
    }
    return nullptr; // nullptr if not found
}

void sortArray(array<Person, 4>& people)
{
    // Sort the array by id
    sort(people.begin(), people.end(), [](const Person& a, const Person& b)
    {
        return a.id < b.id;
    });
}

Person* binarySearchArray(array<Person, 4>& people, const string& id)
{
    sortArray(people);

    int left = 0;
    int right = people.size() - 1;

    while (left <= right)
    {
        int middle = left + (right - left) / 2;
        const Person& person = people[middle];

        if (person.id == id)
        {
            return const_cast<Person*>(&person);
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

    return nullptr; // nullptr if not found
}

vector<Person> getDataAsVector(const json& data) 
{
    vector<Person> people;

    for (const auto& item : data) {
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
            person.children_first_name="N/A";
            person.children_last_name="N/A";
            person.children_email="N/A";
        }

        people.push_back(person);
    }

    return people;
}

Person linearSearchVector(const vector<Person>& people, const string& searchId) 
{
    for (const Person& person : people) 
    {
        if (person.id == searchId) 
        {
            return person;
        }
    }

    Person notFoundPerson; // "not found" person if no such ID
    notFoundPerson.id = "not found";
    return notFoundPerson;
}

void sortVector(vector<Person>& peopleVector) 
{
    // Use a lambda function to compare people by their 'id' field
    sort(peopleVector.begin(), peopleVector.end(), [](const Person& a, const Person& b) {
        return a.id < b.id;
    });
}

int binarySearchVector(vector<Person>& peopleVector, const string& targetId) 
{
    sortVector(peopleVector);

    int left = 0;
    int right = peopleVector.size() - 1;

    while (left <= right) 
    {
        int mid = left + (right - left) / 2;
        if (peopleVector[mid].id == targetId) 
        {
            return mid; // found the target person
        } 
        else if (peopleVector[mid].id < targetId) 
        {
            left = mid + 1; // search right half
        } 
        else 
        {
            right = mid - 1; // search left half
        }
    }

    return -1; // target person not found
}


list<Person> insertDataIntoLinkedList(const json& data) {
    list<Person> people;

    for (const auto& item : data) {
        Person person;

        person.id = item.value("_id", "not found");
        person.first_name = item.value("first_name", "not found");
        person.last_name = item.value("last_name", "not found");
        person.email = item.value("email", "not found");

        if (item.find("children") != item.end()) {
            const json& children = item["children"];
            person.children_first_name = children.value("first_name", "not found");
            person.children_last_name = children.value("last_name", "not found");
            person.children_email = children.value("email", "not found");
        } else {
            person.children_first_name = "N/A";
            person.children_last_name = "N/A";
            person.children_email = "N/A";
        }

        people.push_back(person);
    }

    return people;
}

Person* linearSearchLinkedList(const list<Person>& peopleList, const string& targetID) {
    for (auto& person : peopleList) {
        if (person.id == targetID) {
            return const_cast<Person*>(&person);
        }
    }
    return nullptr; // Return nullptr if not found
}

bool compareByID(const Person& a, const Person& b) {
    return a.id < b.id;
}

Person* binarySearchLinkedList(const list<Person>& peopleList, const string& targetID) {
    list<Person> sortedList = peopleList;
    sortedList.sort(compareByID);

    auto begin = sortedList.begin();
    auto end = sortedList.end();

    while (begin != end) {
        auto mid = next(begin, distance(begin, end) / 2);

        if (mid->id == targetID) {
            return &(*mid);
        } else if (mid->id < targetID) {
            begin = next(mid);
        } else {
            end = mid;
        }
    }

    return nullptr; // Return nullptr if not found
}

list<Person> insertDataIntoDoublyLinkedList(const json& data) {
    list<Person> people;

    for (const auto& item : data) {
        Person person;
        person.id = item.value("_id", "not found");
        person.first_name = item.value("first_name", "not found");
        person.last_name = item.value("last_name", "not found");
        person.email = item.value("email", "not found");

        if (item.find("children") != item.end()) {
            const json& children = item["children"];
            person.children_first_name = children.value("first_name", "not found");
            person.children_last_name = children.value("last_name", "not found");
            person.children_email = children.value("email", "not found");
        } else {
            person.children_first_name = "N/A";
            person.children_last_name = "N/A";
            person.children_email = "N/A";
        }

        people.push_back(person);
    }

    return people;
}

Person* linearSearchDoublyLinkedList(const list<Person>& peopleList, const string& targetID) {
    for (const Person& person : peopleList) {
        if (person.id == targetID) {
            return const_cast<Person*>(&person);
        }
    }
    return nullptr;
}

Person* binarySearchDoublyLinkedList(const list<Person>& peopleList, const string& targetID) {
    list<Person> sortedList = peopleList;  // Copy the list
    sortedList.sort([](const Person& a, const Person& b) { return a.id < b.id; });

    for (const Person& person : sortedList) {
        if (person.id == targetID) {
            return const_cast<Person*>(&person);
        }
    }

    return nullptr; // Return nullptr if not found
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

Person* linearSearchStack(const stack<Person>& peopleStack, const string& searchID) {
    stack<Person> tempStack = peopleStack;
    
    while (!tempStack.empty()) {
        if (tempStack.top().id == searchID) {
            return &tempStack.top();
        }
        tempStack.pop();
    }
    
    return nullptr; // Return nullptr if the ID is not found
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

Person* linearSearchQueue(const queue<Person>& peopleQueue, const string& target) 
{
    queue<Person> tempQueue = peopleQueue; // Create a temporary copy of the queue

    while (!tempQueue.empty()) 
    {
        if (tempQueue.front().id == target) 
        {
            return &(tempQueue.front());
        }
        tempQueue.pop();
    }

    return nullptr; // Return nullptr if the target is not found
}

unordered_map<string, Person> insertDataIntoBinaryTree(const json& data) {
    unordered_map<string, Person> binaryTree;

    for (const auto& person : data) {
        Person p;
        p.id = person.value("_id", "not found");
        p.first_name = person.value("first_name", "not found");
        p.last_name = person.value("last_name", "not found");
        p.email = person.value("email", "not found");

        if (person.find("children") != person.end()) {
            const json& children = person["children"];
            p.children_first_name = children.value("first_name", "not found");
            p.children_last_name = children.value("last_name", "not found");
            p.children_email = children.value("email", "not found");
        } else {
            p.children_first_name = "N/A";
            p.children_last_name = "N/A";
            p.children_email = "N/A";
        }

        binaryTree[p.id] = p; // Use the person's id as the key in the unordered_map
    }

    return binaryTree;
}

void binarySearch(const unordered_map<string, Person>& binaryTree, const string& searchIdBT) {
    auto it = binaryTree.find(searchIdBT);

    if (it != binaryTree.end()) {
        cout << "Person found by ID '" << searchIdBT << "' is " << it->second.first_name << " " << it->second.last_name << endl;
    } else {
        cout << "Person with ID '" << searchIdBT << "' not found" << endl;
    }
}

Person BFS(const unordered_map<string, Person>& binaryTree, const string& searchIdBT) {
    queue<string> q;
    unordered_map<string, bool> visited;

    q.push(searchIdBT);

    while (!q.empty()) {
        string currentId = q.front();
        q.pop();

        auto it = binaryTree.find(currentId);

        if (it != binaryTree.end()) {
            if (visited.find(currentId) == visited.end()) {
                visited[currentId] = true;

                if (currentId == searchIdBT) {
                    return it->second;
                }

                if (it->second.children_first_name != "N/A") {
                    q.push(it->second.children_first_name);
                    q.push(it->second.children_last_name);
                }
            }
        }
    }

    return Person(); // Return an empty Person object if not found
}

Person DFS(const unordered_map<string, Person>& binaryTree, const string& searchIdBT) {
    stack<string> s;
    unordered_map<string, bool> visited;

    s.push(searchIdBT);

    while (!s.empty()) {
        string currentId = s.top();
        s.pop();

        auto it = binaryTree.find(currentId);

        if (it != binaryTree.end()) {
            if (visited.find(currentId) == visited.end()) {
                visited[currentId] = true;

                if (currentId == searchIdBT) {
                    return it->second;
                }

                if (it->second.children_first_name != "N/A") {
                    s.push(it->second.children_first_name);
                    s.push(it->second.children_last_name);
                }
            }
        }
    }

    return Person(); // Return an empty Person object if not found
}

int main() {
    ifstream f("Mdata.json");
    json data = json::parse(f);
    f.close(); 

    string dataStructure;
    cout<<"Choose data structure: 'arr' - 'vec' - 'll' - 'dll' - 'sta' - 'que' - 'bt' - 'ht'"<<endl;
    cin>>dataStructure;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if( dataStructure == "arr")
    {
        cout<<"Using array:"<<endl;
        array<Person, 4> peopleArray = getDataAsArray(data);
        string searchIdArray, arraySA;
        cout<<"Search algorithm: 'L' for Linear search - 'B' for Binary search"<<endl;
        cin>>arraySA;
        cout<<"Id of person to be found: "<<endl;
        cin>>searchIdArray;
        if(arraySA == "L")
        {
            Person* foundPersonArray = linearSearchArray(peopleArray, searchIdArray);
            if (foundPersonArray) 
            {
                cout << "Person found by ID '"<< searchIdArray << "' is " << foundPersonArray->first_name << " " << foundPersonArray->last_name << endl;
                // Add more details of person later
            } 
            else 
            {
                cout << "Person with ID '" << searchIdArray <<"' not found"<< endl;
            }
        }
        else if (arraySA == "B") 
        {
            Person* foundPersonArray = binarySearchArray(peopleArray, searchIdArray);
            if (foundPersonArray)
            {
                cout << "Person found by ID '" << searchIdArray << "' is " <<foundPersonArray->first_name << " " << foundPersonArray->last_name << endl;
                // Add more details of person later
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
            Person foundPersonVector = linearSearchVector(peopleVector, searchIdVector);
            if (foundPersonVector.id == "not found") 
            {
                cout << "Person with ID '"<< searchIdVector << "' not found" << endl;
            } 
            else 
            {
            cout << "Person found by ID '"<< searchIdVector << "' is " << foundPersonVector.first_name <<" "<< foundPersonVector.last_name<<endl;
            // Add more details of person later
            }
        }
        else if (VectorSA == "B")
        {
            int indexVector = binarySearchVector(peopleVector, searchIdVector);
            if (indexVector != -1) 
            {
                cout << "Person found by ID '" << searchIdVector << "' is " << peopleVector[indexVector].first_name<< " "<< peopleVector[indexVector].last_name<< endl;
                // Add more details of person later
            } 
            else 
            {
                cout << "Person with ID '"<< searchIdVector << "' not found" << endl;
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
            Person* resultLL = linearSearchLinkedList(peopleLinkedList, searchIdLL);
            if (resultLL != nullptr) 
            {
                cout << "Person found by ID '" << searchIdLL <<"' is " << resultLL->first_name <<" "<< resultLL->last_name << endl;
                // Add more details of person later
            } 
            else 
            {
                cout << "Person with ID '"<< searchIdLL <<"' not found" << endl;
            }
        }
        else if (LLSA == "B")
        {
            Person* resultLL = binarySearchLinkedList(peopleLinkedList, searchIdLL);
            if (resultLL) 
            {
                cout << "Person found by ID '"<< searchIdLL<< "' is " << resultLL->first_name << " " << resultLL->last_name << endl;
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
            Person* resultDLL = linearSearchDoublyLinkedList(peopleDoublyLinkedList, searchIdDLL);
            if (resultDLL) 
            {
                cout << "Person found by ID '"<<searchIdDLL<< "' is "<< resultDLL->first_name<< " "<< resultDLL->last_name << endl;
            } 
            else 
            {
                cout << "Person with ID " << searchIdDLL << " not found" << endl;
            }
        }
        else if (DLLSA == "B")
        {
            Person* resultDLL = binarySearchDoublyLinkedList(peopleDoublyLinkedList, searchIdDLL);
            if (resultDLL) 
            {
                cout << "Person found by ID '"<<searchIdDLL<< "' is "<< resultDLL->first_name<< " "<< resultDLL->last_name << endl;
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
            Person* resultStack = linearSearchStack(peopleStack, searchIdStack);
            if (resultStack)
            {
                cout << "Person found by ID '" << searchIdStack << "' is " << resultStack->first_name << " "<< resultStack->last_name << endl;
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
            Person* resultQueue = linearSearchQueue(peopleQueue, searchIdQueue);
            if (resultQueue) 
            {
                cout << "Person found by ID '" <<searchIdQueue <<"' is "<< resultQueue->first_name << " " << resultQueue->last_name <<endl;
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
        unordered_map<string, Person> binaryTree = insertDataIntoBinaryTree(data);
        cout << "Using binary tree:" << endl;
        string searchIdBT,BTSA;
        cout<<"Search algorithm: 'BS' for Binary search - 'BF' for Breadth first search - 'DF' for Deoth first search"<<endl;
        cin>>BTSA;
        cout<<"Id of person to be found: "<<endl;
        cin>>searchIdBT;

        if (BTSA == "BS")
        {
            binarySearch(binaryTree, searchIdBT);
        }
        else if (BTSA == "BF")
        {
            Person foundPerson = BFS(binaryTree, searchIdBT);
            if (!foundPerson.id.empty()) {
                cout << "Person found by ID '" << searchIdBT << "' is " << foundPerson.first_name << " " << foundPerson.last_name << endl;
                // Add other details here later
            } else {
                cout << "Person with ID '" << searchIdBT << "' not found" << endl;
            }
        }
        else if (BTSA == "DF")
        {
            Person foundPerson = DFS(binaryTree, searchIdBT);
            if (!foundPerson.id.empty()) {
                cout << "Person found by ID '" << searchIdBT << "' is " << foundPerson.first_name << " " << foundPerson.last_name << endl;
                // Add other details here later
            } else {
                cout << "Person with ID '" << searchIdBT << "' not found" << endl;
            }
        }
        else
        {
            cout<<"No such search algorithm '"<< BTSA <<"'"<<endl;
        }    
    }
    else 
    {
        cout<<"No such data structure "<<dataStructure<<endl;
    }
git
    return 0;
}

#include <iostream>
#include <fstream>
#include "nlohmann\json.hpp"
#include <string>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <chrono>

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

    auto start = chrono::high_resolution_clock::now();

    int left = 0;
    int right = people.size() - 1;

    while (left <= right)
    {
        int middle = left + (right - left) / 2;
        const Person& person = people[middle];

        if (person.id == id)
        {
            auto stop = chrono::high_resolution_clock::now();
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

SearchResultForVector linearSearchVector(const vector<Person>& people, const string& targetId)
{
    auto start = chrono::high_resolution_clock::now();

    for (const Person& person : people)
    {
        if (person.id == targetId)
        {
            auto stop = chrono::high_resolution_clock::now();
            return {person, chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
    }

    Person notFoundPerson;
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
    auto start = chrono::high_resolution_clock::now();

    int left = 0;
    int right = peopleVector.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (peopleVector[mid].id == targetId)
        {
            auto stop = chrono::high_resolution_clock::now();
            return {peopleVector[mid], chrono::duration_cast<chrono::nanoseconds>(stop - start)};
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

    Person notFoundPerson;
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
    auto start = chrono::high_resolution_clock::now();

    auto it = peopleList.begin();
    auto end = peopleList.end();

    while (it != end)
    {
        if (it->id == targetID)
        {
            auto stop = chrono::high_resolution_clock::now();
            return {const_cast<Person*>(&(*it)), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }

        if (it->id < targetID)
        {
            ++it;
        }
        else
        {
            break;
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
    auto start = chrono::high_resolution_clock::now();

    auto itFront = peopleList.begin();
    auto itBack = peopleList.end();
    advance(itBack, -1);

    while (itFront->id <= itBack->id)
    {
        auto mid = next(itFront, distance(itFront, itBack) / 2);

        if (mid->id == targetID)
        {
            auto stop = chrono::high_resolution_clock::now();
            return {const_cast<Person*>(&(*mid)), chrono::duration_cast<chrono::nanoseconds>(stop - start)};
        }
        else if (mid->id < targetID)
        {
            itFront = next(mid);
        }
        else
        {
            itBack = prev(mid);
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

SearchResult linearSearchStack(const stack<Person> &peopleStack, const string &targetId)
{
    stack<Person> tempStack = peopleStack;

    auto start = chrono::high_resolution_clock::now();
    while (!tempStack.empty())
    {
        if (tempStack.top().id == targetId)
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
    queue<Person> tempQueue = peopleQueue;

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

    PersonBT* searchNode(const string& targetId) const
    {
        return search(root, targetId);
    }

    PersonBT* BFS(const string& targetId) const
    {
        return BFS(root, targetId);
    }

    PersonBT* DFS(const string& targetId) const
    {
        return DFS(root, targetId);
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

    PersonBT* search(PersonBT* node, const string& targetId) const
    {
        if (node == nullptr || node->id == targetId)
        {
            return node;
        }

        if (targetId < node->id)
        {
            return search(node->left, targetId);
        }
        else
        {
            return search(node->right, targetId);
        }
    }

    PersonBT* BFS(PersonBT* root, const string& targetId) const
    {
        queue<PersonBT*> q;
        q.push(root);

        while (!q.empty())
        {
            PersonBT* current = q.front();
            q.pop();

            if (current != nullptr)
            {
                if (current->id == targetId)
                {
                    return current;
                }

                q.push(current->left);
                q.push(current->right);
            }
        }

        return nullptr;
    }

    PersonBT* DFS(PersonBT* root, const string& targetId) const
    {
        stack<PersonBT*> s;
        s.push(root);

        while (!s.empty())
        {
            PersonBT* current = s.top();
            s.pop();

            if (current != nullptr)
            {
                if (current->id == targetId)
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

const Person* searchInUnorderedMap(const unordered_map<string, Person>& peopleMap, const string& targetId)
{
    auto it = peopleMap.find(targetId);
    if (it != peopleMap.end())
    {
        return &(it->second);
    }
    else
    {
        return nullptr;
    }
}

void resultFound(const string& targetId, const SearchResult& result, int& totalTime, int& currentRun, int& clockBottleneck,string searchAlgorithm, string dataStructure) 
{

    currentRun++;

    if (result.duration.count() == 0) 
    {
        clockBottleneck++;
        std::cout << "Run " << currentRun << " - clock bottleneck " << endl;
    } 
    else 
    {
        std::cout << "Run " << currentRun << " - " << result.duration.count() << " nanoseconds" << endl;
    }

    totalTime += result.duration.count();
}

void resultFoundVector(const string& targetId, const SearchResultForVector& result, int& totalTime, int& currentRun, int& clockBottleneck,string searchAlgorithm, string dataStructure) 
{

    currentRun++;

    if (result.duration.count() == 0) 
    {
        clockBottleneck++;
        std::cout << "Run " << currentRun << " - clock bottleneck " << endl;
    } 
    else 
    {
        std::cout << "Run " << currentRun << " - " << result.duration.count() << " nanoseconds" << endl;
    }

    totalTime += result.duration.count();
}

void writeResultsToFile(const string& filename, const vector<int>& runTimes, int averageTime) 
{
    ofstream outFile(filename);

    if (outFile.is_open()) 
    {
        for (const auto& runTime : runTimes) 
        {
            outFile << runTime << '\n';
        }

        outFile << averageTime;

        outFile.close();
    } 
    else 
    {
        std::cerr << "Error opening file: " << filename << endl;
    }
}

int main() {
    ifstream f("MockData10000.json");
    json data = json::parse(f);
    f.close();

    string dataStructure;
    int nrOfRuns;

        std::cout<<"Choose data structure:\n'arr' for Array\n'vec' for Vector\n'll' for Linked list\n'dll' for Doubly linked list\n'sta' for Stack\n'que' for Queue\n'bt' for Binary tree\n'ht' for Hash table "<<endl;
        std::cin>>dataStructure; //using std:: for "cout" and "cin" because of ambiguity issues betweeen libraries
        std::cout<<"Nr of repetitions (1-100)"<<endl;
        std::cin>>nrOfRuns;
        bool found = false;
        int currentRun = 0;
        int clockBottleneck = 0;

        if (nrOfRuns < 1 || nrOfRuns > 100)
        {
            std::cout<<"Nr of repetitions out of scope";
        }
        else
        {
             //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if( dataStructure == "arr")
        {
            std::cout<<"Using array:"<<endl;
            array<Person, 10000> peopleArray = getDataAsArray(data);
            string targetIdArray, arraySA;
            std::cout<<"Search algorithm: 'L' for Linear search - 'B' for Binary search "<<endl;
            std::cin>>arraySA;
            std::cout<<"Id of person to be found: "<<endl;
            std::cin>>targetIdArray;
            int totalTimeArrayLinear = 0; //choosing to use int instead of float because the accuracy of float is negligible in nanoseconds
            int totalTimeArrayBinary = 0;
            vector<int> runTimesArrayLinear;
            vector<int> runTimesArrayBinary;

            if(arraySA == "L")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResult resultArray = linearSearchArray(peopleArray, targetIdArray);
                    if (resultArray.person != nullptr)
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdArray << "' is " << resultArray.person->first_name << " " << resultArray.person->last_name << endl;
                            std::cout << "Time taken by LINEAR SEARCH on ARRAY: "<<endl;
                        }
                        found = true;
                
                        resultFound(targetIdArray, resultArray, totalTimeArrayLinear, currentRun, clockBottleneck,"Linear Search", "Array");
                        runTimesArrayLinear.push_back(resultArray.duration.count());
                    }
                    else
                    {
                        std::cout << "Person with ID '" << targetIdArray <<"' not found"<< endl;
                        break;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeArrayLinear = totalTimeArrayLinear/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeArrayLinear<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesArrayLinear, totalTimeArrayLinear);
            }
            else if (arraySA == "B")
            {
                sortArray(peopleArray);
                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResult resultArray = binarySearchArray(peopleArray, targetIdArray);
                    if (resultArray.person != nullptr)
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdArray << "' is " << resultArray.person->first_name << " " << resultArray.person->last_name << endl;
                            std::cout << "Time taken by BINARY SEARCH on ARRAY: " << endl;
                        }
                        found = true;
                        resultFound(targetIdArray, resultArray, totalTimeArrayBinary, currentRun, clockBottleneck,"Binary Search", "Array");
                        runTimesArrayBinary.push_back(resultArray.duration.count());
                        
                    }
                    else
                    {
                        std::cout << "Person with ID '" << targetIdArray << "' not found" << endl;
                        break;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeArrayBinary = totalTimeArrayBinary/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeArrayBinary<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesArrayBinary, totalTimeArrayBinary);

            }
            else {std::cout<<"No such search algorithm: '"<< arraySA <<"'"<<endl;}

        }

     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "vec")
        {
            vector<Person> peopleVector = getDataAsVector(data);
            std::cout<<"Using vector:"<<endl;
            string targetIdVector, VectorSA;
            std::cout<<"Search algorithm: 'L' for Linear search - 'B' for Binary search"<<endl;
            std::cin>> VectorSA;
            std::cout<<"Id of person to be found: "<<endl;
            std::cin>>targetIdVector;
            int totalTimeVectorLinear = 0;
            int totalTimeVectorBinary = 0;
            vector<int> runTimesVectorLinear;
            vector<int> runTimesVectorBinary;

            if(VectorSA == "L")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResultForVector resultVector = linearSearchVector(peopleVector, targetIdVector);
                    if (resultVector.person.id == "not found")
                    {
                        std::cout << "Person with ID '" << targetIdVector << "' not found" << endl;
                    }
                    else
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdVector << "' is " << resultVector.person.first_name << " " << resultVector.person.last_name << endl;
                            std::cout << "Time taken by LINEAR SEARCH on VECTOR: " << endl;
                        }
                        found = true;
                        resultFoundVector(targetIdVector, resultVector, totalTimeVectorLinear, currentRun, clockBottleneck,"Linear Search", "Vector");
                        runTimesVectorLinear.push_back(resultVector.duration.count());
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeVectorLinear = totalTimeVectorLinear/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeVectorLinear<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesVectorLinear, totalTimeVectorLinear);

            }
            else if (VectorSA == "B")
            {
                sortVector(peopleVector);
                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResultForVector resultVector = binarySearchVector(peopleVector, targetIdVector);
                    if (resultVector.person.id == "not found")
                    {
                        std::cout << "Person with ID '" << targetIdVector << "' not found" << endl;
                    }
                    else
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdVector << "' is " << resultVector.person.first_name << " " << resultVector.person.last_name << endl;
                            std::cout << "Time taken by BINARY SEARCH on VECTOR: " <<endl;
                        }
                        found = true;
                        resultFoundVector(targetIdVector, resultVector, totalTimeVectorBinary, currentRun, clockBottleneck,"Binary Search", "Vector");
                        runTimesVectorBinary.push_back(resultVector.duration.count());
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeVectorBinary = totalTimeVectorBinary/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeVectorBinary<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesVectorBinary, totalTimeVectorBinary);
            }
            else{ std::cout<<"No such search algorithm: '"<< VectorSA <<"'"<<endl;}
        }
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "ll")
        {
            list<Person> peopleLinkedList = insertDataIntoLinkedList(data);
            std::cout<<"Using linked list:"<<endl;
            string targetIdLL, LLSA;
            std::cout<<"Search algorithm: 'L' for Linear search - 'B' for Binary search"<<endl;
            std::cin>>LLSA;
            std::cout<<"Id of person to be found: "<<endl;
            std::cin>>targetIdLL;
            int totalTimeLlLinear = 0;
            int totalTimeLlBinary = 0;
            vector<int> runTimesLlLinear;
            vector<int> runTimesLlBinary;

            if(LLSA == "L")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResult resultLL = linearSearchLinkedList(peopleLinkedList, targetIdLL);
                    if (resultLL.person != nullptr)
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdLL << "' is " << resultLL.person->first_name << " " << resultLL.person->last_name << endl;
                            std::cout << "Time taken by LINEAR SEARCH on LINKED LIST: " <<endl;
                        }
                        found = true;
                        resultFound(targetIdLL, resultLL, totalTimeLlLinear, currentRun, clockBottleneck,"Linear Search", "Linked List");
                        runTimesLlLinear.push_back(resultLL.duration.count());
                    }
                        else
                    {
                        std::cout << "Person with ID '"<< targetIdLL <<"' not found" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeLlLinear = totalTimeLlLinear/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeLlLinear<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesLlLinear, totalTimeLlLinear);

            }
            else if (LLSA == "B")
            {
                peopleLinkedList.sort(compareByID);
                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResult resultLL = binarySearchLinkedList(peopleLinkedList, targetIdLL);
                    if (resultLL.person != nullptr)
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdLL << "' is " << resultLL.person->first_name << " " << resultLL.person->last_name << endl;
                            std::cout << "Time taken by BINARY SEARCH on LINKED LIST: " <<endl;
                        }
                        found = true;
                        resultFound(targetIdLL, resultLL, totalTimeLlBinary, currentRun, clockBottleneck,"Binary Search", "Linked List");
                        runTimesLlBinary.push_back(resultLL.duration.count());
                    }
                    else
                    {
                        std::cout << "Person with ID '" << targetIdLL << "' not found" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeLlBinary = totalTimeLlBinary/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeLlBinary<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesLlBinary, totalTimeLlBinary);
            }
            else{ std::cout<<"No such search algorithm: '"<< LLSA <<"'"<<endl;}
        }
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "dll")
        {
            list<Person> peopleDoublyLinkedList = insertDataIntoDoublyLinkedList(data);
            std::cout << "Using doubly linked list:" << endl;
            string targetIdDLL, DLLSA;
            std::cout<<"Search algorithm: 'L' for Linear search - 'B' for Binary search"<<endl;
            std::cin>>DLLSA;
            std::cout<<"Id of person to be found: "<<endl;
            std::cin>>targetIdDLL;
            int totalTimeDllLinear = 0;
            int totalTimeDllBinary = 0;
            vector<int> runTimesDllLinear;
            vector<int> runTimesDllBinary;

            if(DLLSA == "L")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResult resultDLL = linearSearchDoublyLinkedList(peopleDoublyLinkedList, targetIdDLL);
                    if (resultDLL.person != nullptr)
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdDLL << "' is " << resultDLL.person->first_name << " " << resultDLL.person->last_name << endl;
                            std::cout << "Time taken by LINEAR SEARCH on DOUBLY LINKED LIST: " << endl;
                        }
                        found = true;
                        resultFound(targetIdDLL, resultDLL, totalTimeDllLinear, currentRun, clockBottleneck,"Linear Search", "Doubly Linked List");
                        runTimesDllLinear.push_back(resultDLL.duration.count());
                    }
                    else
                    {
                        std::cout << "Person with ID " << targetIdDLL << " not found" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeDllLinear = totalTimeDllLinear/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeDllLinear<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesDllLinear, totalTimeDllLinear);
            }
            else if (DLLSA == "B")
            {
                peopleDoublyLinkedList.sort(compareByID);

                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResult resultDLL = binarySearchDoublyLinkedList(peopleDoublyLinkedList, targetIdDLL);
                    if (resultDLL.person != nullptr)
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdDLL << "' is " << resultDLL.person->first_name << " " << resultDLL.person->last_name << endl;
                            std::cout << "Time taken by BINARY SEARCH on DOUBLY LINKED LIST: " << endl;
                        }
                        found = true;
                        resultFound(targetIdDLL, resultDLL, totalTimeDllBinary, currentRun, clockBottleneck,"Binary Search", "Doubly Linked List");
                        runTimesDllBinary.push_back(resultDLL.duration.count());
                    }
                    else
                    {
                        std::cout << "Person with ID " << targetIdDLL << " not found" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeDllBinary = totalTimeDllBinary/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeDllBinary<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesDllBinary, totalTimeDllBinary);
            }
            else
            {
                std::cout<<"No such search algorithm '"<< DLLSA <<"'"<<endl;
            }
        }
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "sta")
        {
            stack<Person> peopleStack;
            pushDataToStack(data, peopleStack);
            std::cout << "Using stack:" << endl;
            string targetIdStack, stackSA;
            std::cout<<"Search algorithm: 'L' for Linear search"<<endl;
            std::cin>>stackSA;
            std::cout<<"Id of person to be found: "<<endl;
            std::cin>>targetIdStack;
            int totalTimeStackLinear = 0;
            vector<int> runTimesStackLinear;

            if(stackSA == "L")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResult resultStack = linearSearchStack(peopleStack, targetIdStack);
                    if (resultStack.person != nullptr)
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdStack << "' is " << resultStack.person->first_name << " " << resultStack.person->last_name << endl;
                            std::cout << "Time taken by LINEAR SEARCH on STACK: " << endl;
                        }
                        found = true;
                        resultFound(targetIdStack, resultStack, totalTimeStackLinear, currentRun, clockBottleneck,"Linear Search", "Stack");
                        runTimesStackLinear.push_back(resultStack.duration.count());
                    }
                    else
                    {
                        std::cout << "Person with ID '" << targetIdStack << "' not found" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeStackLinear = totalTimeStackLinear/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeStackLinear<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesStackLinear, totalTimeStackLinear);
            }
            else
            {
                std::cout<<"No such search algorithm '"<< stackSA <<"'"<<endl;
            }
        }
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if( dataStructure == "que")
        {
            queue<Person> peopleQueue = getDataAsQueue(data);
            std::cout<<"Using queue: "<<endl;

            string targetIdQueue, queueSA;
            std::cout<<"Search algorithm: 'L' for Linear search"<<endl;
            std::cin>>queueSA;
            std::cout<<"Id of person to be found: "<<endl;
            std::cin>>targetIdQueue;
            int totalTimeQueueLinear = 0;
            vector<int> runTimesQueueLinear;

            if(queueSA == "L")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    SearchResult resultQueue = linearSearchQueue(peopleQueue, targetIdQueue);
                    if (resultQueue.person != nullptr)
                    {
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdQueue << "' is " << resultQueue.person->first_name << " " << resultQueue.person->last_name << endl;
                            std::cout << "Time taken by LINEAR SEARCH on QUEUE: " << endl;
                        }
                        found = true;
                        resultFound(targetIdQueue, resultQueue, totalTimeQueueLinear, currentRun, clockBottleneck,"Linear Search", "Queue");
                        runTimesQueueLinear.push_back(resultQueue.duration.count());
                    }
                    else
                    {
                        std::cout << "Person with ID '" << targetIdQueue <<"' not found" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeQueueLinear = totalTimeQueueLinear/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeQueueLinear<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesQueueLinear, totalTimeQueueLinear);
            }
            else
            {
                std::cout<<"No such search algorithm '"<< queueSA <<"'"<<endl;
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
                //std::cout<<"inserted node"<<endl;
            }

            string targetIdBT,BTSA;
            std::cout<<"Search algorithm: 'BS' for Binary search - 'BF' for Breadth first search - 'DF' for Deoth first search"<<endl;
            std::cin>>BTSA;
            std::cout << "Id of person to be found: " << endl;
            std::cin >> targetIdBT;
            int totalTimeBsBinaryTree = 0;
            int totalTimeBfBinaryTree = 0;
            int totalTimeDfBinaryTree = 0;
            vector<int> runTimesBs;
            vector<int> runTimesBf;
            vector<int> runTimesDf;

            if( BTSA == "BS")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    auto start = chrono::high_resolution_clock::now();
                    PersonBT* foundPerson = binaryTree.searchNode(targetIdBT);
                    auto stop = chrono::high_resolution_clock::now();

                    if (foundPerson != nullptr)
                    {
                        currentRun++;
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdBT << "' is " << foundPerson->first_name << " " << foundPerson->last_name << endl;
                            std::cout << "Time taken by BINARY SEARCH on BINARY TREE: " << endl;
                        }
                        found = true;
                        if (chrono::duration_cast<chrono::nanoseconds>(stop - start).count() == 0)
                        {
                            clockBottleneck++;
                            std::cout << "Run " << currentRun << " - clock bottleneck " << std::endl;
                        }
                        else
                        {
                            std::cout<< "Run " << currentRun << " - " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds" << endl;
                            totalTimeBsBinaryTree = totalTimeBsBinaryTree + chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
                        }
                        runTimesBs.push_back(chrono::duration_cast<chrono::nanoseconds>(stop - start).count());
                        
                    }
                    else
                    {
                        std::cout << "Person with ID '" << targetIdBT << "' not found" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeBsBinaryTree = totalTimeBsBinaryTree/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeBsBinaryTree<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesBs, totalTimeBsBinaryTree);
            }

            else if (BTSA == "BF")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    auto start = chrono::high_resolution_clock::now();
                    PersonBT* foundPerson = binaryTree.BFS(targetIdBT);
                    auto stop = chrono::high_resolution_clock::now();

                    if (foundPerson != nullptr)
                    {
                        currentRun++;
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdBT << "' using BFS is " << foundPerson->first_name << " " << foundPerson->last_name << endl;
                            std::cout << "Time taken by BREADTH-FIRST SEARCH on BINARY TREE: " << endl;
                        }
                        found = true;
                        if (chrono::duration_cast<chrono::nanoseconds>(stop - start).count() == 0)
                        {
                            clockBottleneck++;
                            std::cout << "Run " << currentRun << " - clock bottleneck " << std::endl;
                        }
                        else
                        {
                            std::cout<< "Run " << currentRun << " - " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds" << endl;
                            totalTimeBfBinaryTree = totalTimeBfBinaryTree + chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
                        }
                        runTimesBf.push_back(chrono::duration_cast<chrono::nanoseconds>(stop - start).count());
                        
                    }
                    else
                    {
                        std::cout << "Person with ID '" << targetIdBT << "' not found using BFS" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeBfBinaryTree = totalTimeBfBinaryTree/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeBfBinaryTree<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesBf, totalTimeBfBinaryTree);

            }

            else if (BTSA == "DF")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    auto start = chrono::high_resolution_clock::now();
                    PersonBT* foundPerson = binaryTree.DFS(targetIdBT);
                    auto stop = chrono::high_resolution_clock::now();

                    if (foundPerson != nullptr)
                    {
                        currentRun++;
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdBT << "' using DFS is " << foundPerson->first_name << " " << foundPerson->last_name << endl;
                            std::cout << "Time taken by DEPTH-FIRST SEARCH on BINARY TREE: " << endl;
                        }
                        found = true;
                        if (chrono::duration_cast<chrono::nanoseconds>(stop - start).count() == 0)
                        {
                            clockBottleneck++;
                            std::cout << "Run " << currentRun << " - clock bottleneck " << std::endl;
                        }
                        else
                        {
                            std::cout<< "Run " << currentRun << " - " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds" << endl;
                            totalTimeDfBinaryTree = totalTimeDfBinaryTree + chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
                        }
                        runTimesDf.push_back(chrono::duration_cast<chrono::nanoseconds>(stop - start).count());
                    }
                    else
                    {
                        std::cout << "Person with ID '" << targetIdBT << "' not found using DFS" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeDfBinaryTree = totalTimeDfBinaryTree/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeDfBinaryTree<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesDf, totalTimeDfBinaryTree);
            }

            else {std::cout<<"No such search algorithm '"<< BTSA <<"'"<<endl;}
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (dataStructure == "ht")
        {
            unordered_map<string, Person> peopleMap = insertDataIntoUnorderedMap(data);

            string targetIdHT,HTSA;
            std::cout<<"Search algorithm: 'HS' for Hash search"<<endl;
            std::cin>>HTSA;
            std::cout<<"Id of person to be found: "<<endl;
            std::cin>>targetIdHT;
            int totalTimeHtHashSearch = 0;
            vector<int> runTimesHt;

            if (HTSA == "HS")
            {
                for(int i = 0; i<nrOfRuns; i++)
                {
                    auto start = chrono::high_resolution_clock::now();
                    const Person* resultMap = searchInUnorderedMap(peopleMap, targetIdHT);
                    auto stop = chrono::high_resolution_clock::now();
                    if (resultMap)
                    {
                        currentRun++;
                        if (found == false)
                        {
                            std::cout << "Person found by ID '" << targetIdHT << "' is " << resultMap->first_name << " " << resultMap->last_name << endl;
                            std::cout << "Time taken by HASH-SEARCH on UNORDERED MAP: " << endl;
                        }
                        found = true;
                        if(chrono::duration_cast<chrono::nanoseconds>(stop - start).count() == 0)
                        {
                            clockBottleneck++;
                            std::cout << "Run " << currentRun << " - clock bottleneck " << std::endl;
                        }
                        else
                        {
                            std::cout<< "Run " << currentRun << " - " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds" << endl;
                            totalTimeHtHashSearch = totalTimeHtHashSearch + chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
                        }
                        runTimesHt.push_back(chrono::duration_cast<chrono::nanoseconds>(stop - start).count());
                    }
                    else
                    {
                        std::cout << "Person with ID '" << targetIdHT << "' not found" << endl;
                    }
                }
                nrOfRuns= nrOfRuns - clockBottleneck;
                totalTimeHtHashSearch = totalTimeHtHashSearch/nrOfRuns;
                std::cout<<"Average time of all runs: "<<totalTimeHtHashSearch<< " nanoseconds" <<endl;
                writeResultsToFile("python/results.txt", runTimesHt, totalTimeHtHashSearch);
            }
            else{std::cout<<"No such search algorithm '"<< HTSA <<"'"<<endl;}

        }

        else
        {
            std::cout<<"No such data structure "<<dataStructure<<endl;
        }
        }



    return 0;
}

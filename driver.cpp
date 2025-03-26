// UMBC - CMSC 341 - Spring 2024 - Proj3
#include "rqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>     //used to get the current time
using namespace std;
// We can use the Random class to generate the test data randomly!
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
// Priority functions compute an integer priority for a student.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Student &student);
int priorityFn2(const Student &student);
// a name database for testing purposes
// we can use this DB to generate student names randomly instead of generating
// random strings as names (from Random class)
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
    "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
    "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green", 
    "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong", 
    "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
    "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};
class Tester{
    // implement your test functions in this class
    public:
        bool testMergeDifferent();
        bool testDequeueEmpty();
        bool testAssignmentOpNormal();
        bool testAssignmentOpEdge();
        bool testCopyConstructorNormal();
        bool testCopyConstructorEdge();
        bool testMergeEmptyWithNormal();
        bool testPriorityFuncChange();
};
int main(){
    Random nameGen(97,122);Random levelGen(0,3);
    Random majorGen(0,4);Random groupGen(0,3);
    Random raceGen(0,2);Random genderGen(0,2);
    Random incomeGen(0,4);Random highschoolGen(0,2);
    RQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    Tester tester;
    for (int i=0;i<8;i++){
        Student student(nameGen.getRandString(5), levelGen.getRandNum(),
                    majorGen.getRandNum(), groupGen.getRandNum(),
                    raceGen.getRandNum(), genderGen.getRandNum(),
                    incomeGen.getRandNum(), highschoolGen.getRandNum());
        aQueue.insertStudent(student);
    }
    cout << "\nDump of the leftist heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    aQueue.setStructure(SKEW);
    cout << "\nDump of the skew heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn2 (MINHEAP):\n";
    aQueue.printStudentsQueue();
    aQueue.setPriorityFn(priorityFn1, MAXHEAP);
    cout << "\nDump of the skew queue with priorityFn1 (MAXHEAP):\n";
    aQueue.dump();

    // *************** testing below **************** //
    cout << endl;
    cout << "Attempting to merge queues with different priority functions..." << endl;
    if(tester.testMergeDifferent())
    {cout << "domain_error exception CAUGHT! test PASSED!" << endl;}
    else{cout << "domain_error exception NOT CAUGHT! test FAILED!" << endl;}

    cout << endl;
    cout << "Testing the copy constructor for a normal case..." << endl;
    if(tester.testCopyConstructorNormal())
    {cout << "test PASSED!" << endl;}
    else{cout << "test FAILED!" << endl;}

    cout << endl;
    cout << "Testing the copy constructor for an edge case..." << endl;
    if(tester.testCopyConstructorEdge())
    {cout << "test PASSED!" << endl;}
    else{cout << "test FAILED!" << endl;}

    cout << endl;
    cout << "Testing the assignment operator for an edge case..." << endl;
    if(tester.testAssignmentOpEdge())
    {cout << "test PASSED!" << endl;}
    else{cout << "test FAILED!" << endl;}

    cout << endl;
    cout << "Testing the assignment operator for a normal case..." << endl;
    if(tester.testAssignmentOpNormal())
    {cout << "Test PASSED!" << endl;}
    else{cout << "Test FAILED" << endl;}

    cout << endl;
    cout << "Attempting to dequeue an empty queue ...." << endl;
    if(tester.testDequeueEmpty())
    {cout << "out_of_range exception CAUGHT! test PASSED!" << endl;}
    else{cout << "out_of_range exception NOT CAUGHT! test FAILED!" << endl;}

    cout << endl;
    cout << "Testing merge of an empty queue with a normal queue using mergeWithQueue(RQueue& rhs)..." << endl;
    if(tester.testMergeEmptyWithNormal())
    {cout << "test PASSED!" << endl;}
    else{cout << "test FAILED!" << endl;}

    cout << endl;
    cout << "Testing whether after changing the priority function a correct heap is rebuilt and the different priority function..." << endl;
    if(tester.testPriorityFuncChange())
    {cout << "test PASSED!" << endl;}
    else{cout << "test FAILED!" << endl;}


    return 0;
}
int priorityFn1(const Student & student) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-10]
    //level + major + group
    //the highest priority would be 3+4+3 = 10
    //the lowest priority would be 0+0+0 = 0
    //the larger value means the higher priority
    int priority = student.getLevel() + student.getMajor() + student.getGroup();
    return priority;
}

int priorityFn2(const Student & student) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-10]
    //race + gender + income + highschool
    //the highest priority would be 0+0+0+0 = 0
    //the lowest priority would be 2+2+4+2 = 10
    //the smaller value means the higher priority
    int priority = student.getRace() + student.getGender() + student.getIncome() + student.getHighschool();
    return priority;
}

bool Tester::testMergeDifferent() {
    // Create two queues with different priority functions
    RQueue queue1(priorityFn1, MINHEAP, SKEW);
    RQueue queue2(priorityFn2, MINHEAP, SKEW);

    // Add some dummy data to the queues
    Student student1("John", 1, 1, 1, 1, 1, 1, 1);
    Student student2("Alice", 2, 2, 2, 2, 2, 2, 2);
    queue1.insertStudent(student1);
    queue2.insertStudent(student2);

    // Attempt to merge the queues
    try {
        queue1.mergeWithQueue(queue2);
        // If no exception is thrown, the test fails
        return false;
    } catch (const domain_error& e) {
        // Exception caught, test passed
        return true;
    }
}

bool Tester::testDequeueEmpty() {
    // Create an empty queue
    RQueue queue(priorityFn1, MINHEAP, SKEW);
    // Attempt to dequeue from the empty queue
    try {
        queue.getNextStudent();
        // If no exception is thrown, the test fails
        return false;
    } catch (const out_of_range& e) {
        // Exception caught, test passed
        return true;
    }
}

bool Tester::testAssignmentOpNormal() {
    // create a queue
    RQueue startQueue(priorityFn1, MINHEAP, SKEW);
    // add some students
    Student student1("Mike", 1, 1, 1, 1, 1, 1, 1);
    Student student2("Jack", 2, 2, 2, 2, 2, 2, 2);
    Student student3("Hammer", 3, 3, 3, 3, 3, 3, 3);
    startQueue.insertStudent(student1);
    startQueue.insertStudent(student2);
    startQueue.insertStudent(student3);
    // create another  queue
    RQueue anotherQueue(priorityFn1, MINHEAP, SKEW);
    // add a student
    Student student4("Baby", 1, 2, 1, 2, 1, 2, 1);
    anotherQueue.insertStudent(student3);
    startQueue = anotherQueue;
    int num = 0;
    // compare values in both queues
    while (!anotherQueue.empty() and !startQueue.empty()){
        if (priorityFn2(anotherQueue.getNextStudent()) == priorityFn2(startQueue.getNextStudent())){
            num++;
        }
    }
    // if all 1 students match, the op passed
    if (num == 1){
        return true;
    }
    return false;
}

bool Tester::testAssignmentOpEdge() {
    // create a queue
    RQueue startQueue(priorityFn1, MINHEAP, SKEW);
    // add a student
    Student student1("Mike", 1, 1, 1, 1, 1, 1, 1);
    Student student2("Jack", 2, 2, 2, 2, 2, 2, 2);
    Student student3("Hammer", 3, 3, 3, 3, 3, 3, 3);
    startQueue.insertStudent(student1);
    startQueue.insertStudent(student2);
    startQueue.insertStudent(student3);
    // create an empty queue
    RQueue emptyQueue(priorityFn1, MINHEAP, SKEW);
    emptyQueue = startQueue;
    int num = 0;
    // compare values in both queues
    while (!emptyQueue.empty() and !startQueue.empty()){
        if (priorityFn2(emptyQueue.getNextStudent()) == priorityFn2(startQueue.getNextStudent())){
            num++;
        }
    }
    // if all 3 students match, the op passed
    if (num == 3){
        return true;
    }
    return false;
}

bool Tester::testCopyConstructorNormal() {
    // create an RQueue objects with some data
    RQueue startQueue(priorityFn1, MINHEAP, SKEW);
    Student student1("Mike", 1, 1, 1, 1, 1, 1, 1);
    Student student2("Jack", 2, 2, 2, 2, 2, 2, 2);
    Student student3("Hammer", 3, 3, 3, 3, 3, 3, 3);
    startQueue.insertStudent(student1);
    startQueue.insertStudent(student2);
    startQueue.insertStudent(student3);
    // create a copy of the original queue
    RQueue startCopy(startQueue);
    //bool same = startQueue == startCopy; (doesnt work)
    return false;
}

bool Tester::testCopyConstructorEdge() {
    // create an empty queue
    RQueue startQueue(priorityFn1, MINHEAP, SKEW);
    // create a copy of the original queue
    RQueue startCopy(startQueue);
    //bool same = startQueue == startCopy; (doesnt work)
    return false;
}

bool Tester::testMergeEmptyWithNormal() {
    // create an empty queue
    RQueue emptyQueue(priorityFn1, MINHEAP, SKEW);
    // create a populated queue
    RQueue startQueue(priorityFn1, MINHEAP, SKEW);
    Student student1("Mike", 1, 1, 1, 1, 1, 1, 1);
    Student student2("Jack", 2, 2, 2, 2, 2, 2, 2);

    RQueue normalQueueCopy = startQueue;
    emptyQueue.mergeWithQueue(startQueue);

    int num = 0;
    // compare values in both queues
    while (!emptyQueue.empty() and !normalQueueCopy.empty()) {
        if (priorityFn2(emptyQueue.getNextStudent()) == priorityFn2(startQueue.getNextStudent())) {
            num++;
        }
    }
    if (num == 2){
        return true;
    }
    return false;
}

bool Tester::testPriorityFuncChange() {
    RQueue startQueue(priorityFn1, MINHEAP, SKEW);
    Student student1("Mike", 1, 1, 1, 1, 1, 1, 1);
    Student student2("Jack", 2, 2, 2, 2, 2, 2, 2);
    Student student3("Hammer", 3, 3, 3, 3, 3, 3, 3);
    startQueue.insertStudent(student1);
    startQueue.insertStudent(student2);
    startQueue.insertStudent(student3);
    RQueue originalQueue = startQueue;

    startQueue.setPriorityFn(priorityFn2, MAXHEAP);
    if (startQueue.getHeapType() == MAXHEAP) {
        if (startQueue.getPriorityFn() != originalQueue.getPriorityFn()) {
            return true;
        }
        return false;
    }
    return false;
}
// UMBC - CMSC 341 - Spring 2024 - Proj3
#include "rqueue.h"
// Constructor
// Performs necessary initializations
RQueue::RQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) {
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
}

// Destructor
RQueue::~RQueue() {
    // calls clear function to deallocate memory
  clear();
}

// Clears the queue and deletes all nodes in the heap
// re-initializes the member variables.
void RQueue::clear() {
    // call helper function to delete all nodes in the heap
    clear(m_heap);
    // re-initialize member variables
    m_heap = nullptr;
    m_size = 0;
}

// Clear helper to delete all nodes recursively
void RQueue::clear(Node* cNode) {
    if (cNode != nullptr){
        // trav left
        clear(cNode->m_left);
        // trav right
        clear(cNode->m_right);
        // delete curr node cNode
        delete cNode;
    }
}

// Copy constructor
RQueue::RQueue(const RQueue& rhs) {
    // initialize member variables
    m_heap = nullptr;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_size = rhs.m_size;
    // copy nodes from rhs
    if (rhs.m_heap != nullptr){
        copyHelp(rhs.m_heap);
    }
}

// Copy constructor helper
void RQueue::copyHelp(Node* rhsNode) {
    if (rhsNode == nullptr){
        return;
    }
    Student studentCopy = rhsNode->getStudent();
    insertStudent(studentCopy);

    // copy left and right trees recursively
    copyHelp(rhsNode->m_left);
    copyHelp(rhsNode->m_right);
}

// Returns the heap type (MINHEAP/MAXHEAP).
HEAPTYPE RQueue::getHeapType() const {
  return m_heapType;
}

// Overloaded assignment operator
RQueue& RQueue::operator=(const RQueue& rhs) {
    // check for self-assignment
    if (this != &rhs) {
        // clear the current queue
        clear();
        m_priorFunc = rhs.m_priorFunc;
        m_heapType = rhs.m_heapType;
        m_structure = rhs.m_structure;
        m_size = rhs.m_size;
        // perform a deep copy of the heap
        if (rhs.m_heap != nullptr) {
            m_heap = deepCopy(rhs.m_heap);
        } else {
            m_heap = nullptr;
        }
        return *this;
    }
    return *this;
}

// Assignment operator helper
Node* RQueue::deepCopy(const Node *rhsNode) {
    if (rhsNode == nullptr) {
        return nullptr;
    }
    // create a new node with the same data
    Node* newNode = new Node(rhsNode->m_student);

    // use recursion to traverse left and right subtrees
    newNode->m_left = deepCopy(rhsNode->m_left);
    newNode->m_right = deepCopy(rhsNode->m_right);

    // also copy npl  if a leftist heap
    if (getStructure() == LEFTIST){
        newNode->setNPL(rhsNode->getNPL());
    }
    // return new node
    return newNode;
}

// Merge function to merge host queue to the rhs
void RQueue::mergeWithQueue(RQueue& rhs) {
    if (this == &rhs) {
        throw std::domain_error("Cannot merge a queue with itself! ");
    }
    if (getPriorityFn() != rhs.m_priorFunc) {
        throw std::domain_error("Cannot merge queues with different priority functions! ");
    }
    if (getStructure() != rhs.m_structure) {
        throw std::domain_error("Cannot merge queues with different data structures! ");
    }
    // call private merge helper to merge 2 heaps
    m_heap = merge(m_heap, rhs.m_heap);
    // update size
    m_size += rhs.m_size;
    // clear rhs
    rhs.clear();
}

// handles insertion of a new student
void RQueue::insertStudent(const Student& student) {
    // allocate data for a new student
    Node* newNode = new Node(student);
    if (empty()){
        m_heap = newNode;
    }else{
        m_heap = merge(m_heap, newNode);
        if (m_structure = LEFTIST){
            updateNPL(m_heap);
            }
        }
    m_size++;
}

// Return the current number of students in the queue
int RQueue::numStudents() const{
    return m_size;
}

// Returns the current priority function
prifn_t RQueue::getPriorityFn() const {
  return m_priorFunc;
}

// Remove the node and return the highest priority student from the queue
Student RQueue::getNextStudent() {
    if (empty()){
        throw std::out_of_range("Queue is empty");
    }
    // store highest priority student
    Student highestPStudent = m_heap->m_student;

    // remove the highest priority node
    m_heap = deleteStudent(m_heap);
    // decrement queue size
    m_size--;

    // return the highest priority student from the queue
    return highestPStudent;
}

// This function sets a new priority function and its corresponding heap type
void RQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    // set new priority function and its corresponding heap type
    m_priorFunc = priFn;
    m_heapType = heapType;
    // call helper function to rebuild the heap
    heapRebuild(m_heap);
}

// Sets the data structure and rebuilds according to new struct (SKEW/LEFTIST).
void RQueue::setStructure(STRUCTURE structure){
    // the structure does not change
    if (m_structure == structure){
        return;
    }
    // update structure
    m_structure = structure;
    if (!empty()){
        // if the current heap is empty
        heapRebuild(m_heap);
    }
}

// Returns the structure of the current heap (SKEW/LEFTIST).
STRUCTURE RQueue::getStructure() const {
    return m_structure;
}

// Print using post order traversal?
void RQueue::printStudentsQueue() const {
    cout << "Contents of the queue:" << endl;
    // call print helper function
    printHelp(m_heap);
}

// Helper to print students using post order traversal
void RQueue::printHelp(Node* cNode) const {
    if (cNode != nullptr){
        cout << "[" << m_priorFunc(cNode->m_student) << "] " << cNode->m_student << endl;
        printHelp(cNode->m_left);
        printHelp(cNode->m_right);
    }
}

// Provided functions below
void RQueue::dump() const {
    if (m_size == 0) {
        cout << "Empty heap.\n" ;
    } else {
        dump(m_heap);
    }
    cout << endl;
}

void RQueue::dump(Node *pos) const {
    if ( pos != nullptr ) {
        cout << "(";
        dump(pos->m_left);
        if (m_structure == SKEW)
            cout << m_priorFunc(pos->m_student) << ":" << pos->m_student.m_name;
        else
            cout << m_priorFunc(pos->m_student) << ":" << pos->m_student.m_name << ":" << pos->m_npl;
        dump(pos->m_right);
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Student& student) {
    sout << "Student name: " << student.m_name
        << ", Major: " << student.getMajorStr()
        << ", Gender: " << student.getGenderStr()
        << ", Level: " << student.getLevelStr();
    return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
    sout << node.m_student;
    return sout;
}

/*****************************************
* Private helper function implementation *
******************************************/

// Helper to update
void RQueue::updateNPL(Node *cNode) {
    if (cNode == nullptr) {
        return;
    }
    // Recursively update the NPL of left and right children
    updateNPL(cNode->m_left);
    updateNPL(cNode->m_right);

    // Update the NPL of the current node
    cNode->m_npl = (cNode->m_right == nullptr) ? 0 : cNode->m_right->m_npl + 1;
}

// Returns true if the heap is empty
bool RQueue::empty() {
    if (m_heap == nullptr){
        return true;
    }
    return false;
}

// Removes the elem with the largest key
Node* RQueue::deleteStudent(Node *cNode) {
    if (cNode == nullptr) {
        return nullptr;
    }
    // store left and right child
    Node *leftChild = cNode->m_left;
    Node *rightChild = cNode->m_right;
    // delete root node
    delete cNode;
    // Merge the left and right subtrees
    return merge(leftChild, rightChild);
}

// Private skew merge function
Node* RQueue::merge(Node *heap1, Node *heap2) {
    // If heap1 is null, return heap2
    if (heap1 == nullptr) {
        return heap2;
    }
    // If heap2 is null, return heap1
    if (heap2 == nullptr) {
        return heap1;
    }
    // If heap1's minimum value is less than or equal to heap2's minimum value
    if (m_priorFunc(heap1->getStudent()) <= m_priorFunc(heap2->getStudent())) {
        // Swap to maintain heap property
        Node *temp = heap1->m_right;
        heap1->m_right = heap1->m_left;
        heap1->m_left = merge(heap2, temp); // Recursively merge heap2 with the previous right subtree of heap1
        return heap1;
    } else {
        // Recursively merge heap2 with heap1
        return merge(heap2, heap1);
    }
}

// Helper to rebuild heap
void RQueue::heapRebuild(Node* cNode) {
    if (cNode == nullptr) {
        return;
    }

    // Traverse the heap and rebuild it according to the new structure
    if (getHeapType() == MINHEAP){
        m_heap = buildMinHeap(m_heap);
    }else if (getHeapType() == MAXHEAP){
        m_heap = buildMaxHeap(m_heap);
    }

    // Swap left and right children if the new structure is LEFTIST
    if (m_structure == LEFTIST) {
        Node* temp = cNode->m_left;
        cNode->m_left = cNode->m_right;
        cNode->m_right = temp;
    }
}

Node* RQueue::buildMinHeap(Node* cNode){
    if (cNode == nullptr) {
        return nullptr;
    }

    Node* left = buildMinHeap(cNode->m_left);
    Node* right = buildMinHeap(cNode->m_right);

    // Adjust the heap to satisfy the min-heap property
    if (left != nullptr && m_priorFunc(left->m_student) < m_priorFunc(cNode->m_student)) {
        swap(cNode->m_student, left->m_student);
    }
    if (right != nullptr && m_priorFunc(right->m_student) < m_priorFunc(cNode->m_student)) {
        swap(cNode->m_student, right->m_student);
    }
    return cNode;
}

Node* RQueue::buildMaxHeap(Node* cNode){
    if (cNode == nullptr) {
        return nullptr;
    }

    Node* left = buildMaxHeap(cNode->m_left);
    Node* right = buildMaxHeap(cNode->m_right);

    // Adjust the heap to satisfy the max-heap property
    if (left != nullptr && m_priorFunc(left->m_student) > m_priorFunc(cNode->m_student)) {
        swap(cNode->m_student, left->m_student);
    }
    if (right != nullptr && m_priorFunc(right->m_student) > m_priorFunc(cNode->m_student)) {
        swap(cNode->m_student, right->m_student);
    }
    return cNode;
}
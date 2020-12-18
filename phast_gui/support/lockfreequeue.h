// Code is from Herb Sutter (2008):
//  http://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448?pgno=2 (accessed on 2018-03-31)
// and is assumed to be in the public domain.

#ifndef LOCKFREEQUEUE_H
#define LOCKFREEQUEUE_H

#include <atomic>

template <typename T>
class LockFreeQueue {
private:
  struct Node {
    explicit Node( T val ) : value(val), next(nullptr) { }
    T value;
    Node* next;
  };
  Node* first;             // for producer only
  std::atomic<Node*> divider, last;         // shared

public:
  LockFreeQueue() :
    first(new Node( T() )),
    divider(first),
    last(first)
  {
  }

  ~LockFreeQueue() {
    while( first != nullptr ) {   // release the list
      Node* tmp = first;
      first = tmp->next;
      delete tmp;
    }
  }

  LockFreeQueue(const LockFreeQueue&) = delete;
  LockFreeQueue& operator=(const LockFreeQueue&) = delete;

  void Produce( const T& t ) {
    last.load()->next = new Node(t);    // add the new item
        last.store( last.load()->next );      // publish it

        Node* div = divider.load();
        while( first != div ) { // trim unused nodes
          Node* tmp = first;
          first = first->next;
          delete tmp;
        }
  }

  template <typename ForwardIterator>
  void ProduceMany(ForwardIterator start,
                   ForwardIterator end)
  {
      Node* curr_last = this->last.load();

      for (auto it = start; it != end; it++) { // This loop takes a lot of time!
            Node* new_node = new Node(*it);
            curr_last->next = new_node;
            curr_last = new_node;
      }

      this->last = curr_last;

      Node* div = divider.load();

      while( first != div ) { // trim unused nodes. This loop takes a lot of time!
        Node* tmp = first;
        first = first->next;
        delete tmp;
      }
  }

  bool Consume( T& result ) {
      if( divider != last ) {         // if queue is nonempty
        result = divider.load()->next->value;  // C: copy it back // TODO: crashes here when we shutdown the application
        divider = divider.load()->next;   // D: publish that we took it
        return true;              // and report success
      }
      return false;               // else report empty
    }
};

#endif // LOCKFREEQUEUE_H

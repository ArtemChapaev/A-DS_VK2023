#include<iostream>
#include<cassert>
#include<sstream>

template<class T>
class Queue {
public:
	Queue() : head( nullptr ), tail( nullptr ) {}
	~Queue();

	Queue( Queue& ) = delete;
	Queue& operator=( const Queue& ) = delete;

	void Enqueue( const T& data );
	T Dequeue();
	bool IsEmpty() const;

private:

	// head->  1 -> 2 -> 3 -> 4  <-tail 
	struct Node {
		Node* Next;
		T Data;
		Node() : Next( nullptr ) {}
	};
	Node* head;
	Node* tail;
};

template<class T>
Queue<T>::~Queue()
{
	while( !IsEmpty() ) {
		Dequeue();
	}
}

template<class T>
void Queue<T>::Enqueue( const T& data )
{
	Node* node = new Node;
	node->Data = data;

	if( IsEmpty() ) {
		head = tail = node;
	} else {
		tail->Next = node;
		tail = node;
	}
}

template<class T>
T Queue<T>::Dequeue()
{
	assert( !IsEmpty() );

	T resultData = head->Data;
	Node* nodeToDelete = head;
	head = head->Next;
	if( head == nullptr ) {
		tail = nullptr;
	}
	delete nodeToDelete;
	return resultData;
}

template<class T>
bool Queue<T>::IsEmpty() const
{
	return head == nullptr;
}

void run( std::istream& input, std::ostream& ouput )
{
	Queue<int> queue;
	int n = 0;
	input >> n;
	bool result = true;
	for( int i = 0; i < n; ++i ) {
		int command = 0;
		int data = 0;
		input >> command >> data;
		switch( command ) {
			case 2:
				if( queue.IsEmpty() ) {
					result = result && data == -1;
				} else {
					result = result && queue.Dequeue() == data;
				}
				break;
			case 3:
				queue.Enqueue( data );
				break;
		}
	}

	ouput << (result ? "YES" : "NO");
}

void testQueue()
{
	{
		std::stringstream input;
		std::stringstream output;
		input << "3 3 44 3 50 2 44";
		run( input, output );
		assert( output.str() == "YES" );
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "2 2 -1 3 10";
		run( input, output );
		assert( output.str() == "YES");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "2 3 44 2 66";
		run( input, output );
		assert( output.str() == "NO" );
	}
}

int main()
{
	run( std::cin, std::cout );
//	testQueue();
	return 0;
}
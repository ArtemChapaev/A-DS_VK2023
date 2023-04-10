#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

struct StringHasher {
	unsigned int operator()( const string& key ) const {
		unsigned int hash = 0;
		for( unsigned int i = 0; i < key.size(); ++i ) {
			hash = hash * 137 + key[i];
		}
		return hash;
	}
};

template<class T, class H>
class HashTable {
public:
	HashTable( int initialSize, const H& hasher );
	~HashTable();

	// Key: key = Key ? return true : пробируемся дальше
	// Deleted: пробируемся дальше
	// Empty: return false
	bool Has( const T& key ) const;
	// Key: key = Key ? return false : пробируемся дальше
	// Deleted: встречали ранее Deleted ? идём дальше : запоминаем позицию firstDeleted и идём дальше
	// Empty: вставляем ключ в firstDeleted (если такой был) или в текущую позицию и return true
	bool Add( const T& key );
	// Key: key == Key ? помечаем ячейку Deleted и return true : пробируемся дальше
	// Deleted: пробируемся дальше
	// Empty: return false
	bool Delete( const T& key );

private:
	H hasher;
	struct HashTableNode {
		T Key;
		unsigned int Hash;
		HashTableNode* Next;
		
		HashTableNode() : Hash(0), Next( nullptr ) {}
		HashTableNode( const T& key, unsigned int hash, HashTableNode* next ) :
			Key( key), Hash( hash ), Next( next ) {}
	};
	vector<HashTableNode*> table;
	//struct HashTableNode {
	//	T Key;
	//	enum State { Empty, Key, Deleted };
	//}
	//vector<HashTableNode> table;

	unsigned int keysCount;

	void growTable();
};

template<class T, class H>
HashTable<T, H>::HashTable( int initialSize, const H& _hasher ) :
	hasher( _hasher ),
	table( initialSize, nullptr ),
	keysCount( 0 )
{
}

template<class T, class H>
HashTable<T, H>::~HashTable()
{
	for( int i = 0; i < table.size(); ++i ) {
		HashTableNode* node = table[i];
		while( node != nullptr ) {
			HashTableNode* nextNode = node->Next;
			delete node;
			node = nextNode;
		}
	}
}

template<class T, class H>
bool HashTable<T, H>::Has( const T& key ) const
{
	unsigned int hash = hasher( key ) % table.size();
	HashTableNode* node = table[hash];
	while( node != nullptr && node->Key != key ) {
		node = node->Next;
	}
	
	return node != nullptr;
}

template<class T, class H>
bool HashTable<T, H>::Add( const T& key )
{
	if( keysCount > 3 * table.size() ) {
		growTable();
	}

	unsigned int absHash = hasher( key );
	unsigned int hash = absHash % table.size();
	HashTableNode* node = table[hash];
	while( node != nullptr && node->Key != key ) {
		node = node->Next;
	}

	if( node != nullptr ) {
		return false;
	}

	HashTableNode* newNode = new HashTableNode( key, absHash, table[hash] );
	table[hash] = newNode;
	++keysCount;
	return true;
}

template<class T, class H>
bool HashTable<T, H>::Delete( const T& key )
{
	unsigned int hash = hasher( key ) % table.size();
	HashTableNode* node = table[hash];
	HashTableNode* prevNode = nullptr;
	while( node != nullptr && node->Key != key ) {
		prevNode = node;
		node = node->Next;
	}

	if( node == nullptr ) {
		return false;
	}

	if( prevNode == nullptr ) {
		table[hash] = node->Next;
	} else {
		prevNode->Next = node->Next;
	}

	delete node;
	--keysCount;
	return true;
}

template<class T, class H>
void HashTable<T, H>::growTable()
{
	vector<HashTableNode*> newTable( table.size() * 2, nullptr );
	for( int i = 0; i < table.size(); ++i ) {
		HashTableNode* node = table[i];
		while( node != nullptr ) {
			HashTableNode* nextNode = node->Next;
			unsigned int newHash = node->Hash % newTable.size();
			node->Next = newTable[newHash];
			newTable[newHash] = node;
			node = nextNode;
		}
	}
	table = std::move( newTable );
}

int main()
{
	StringHasher h;
	HashTable<string, StringHasher> table( 1, h );
	char operation = 0;
	string word;
	while( std::cin >> operation >> word ) {
		switch( operation ) {
			case '+':
				std::cout << ( table.Add( word ) ? "OK" : "FAIL" ) << std::endl;
				break;
			case '-':
				std::cout << (table.Delete( word ) ? "OK" : "FAIL") << std::endl;
				break;
			case '?':
				std::cout << (table.Has( word ) ? "OK" : "FAIL") << std::endl;
				break;
		}
	}
	return 0;
}
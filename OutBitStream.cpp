/*
file: тра-тата*тра-тата

A -----------------------------------------> B

Unicode
UTF-32
1 буква - 4 байта
UTF-8
1 буква - 1-4 байта
UTF-16
1 буква - 2 или 4 байта
BOM

17 * 8 = 136 бита
закодированная таблица + код (51 бит)
сколько букв + (BYTE + ci) * N + код (51 бит) = 112 бит
   1 байт    + (11бит) * 5 + 51 


сколько букв + (BYTE + BYTE + ci) * N + код( 37 бит ) = 137 бит
      8            3*18 + 2*19 + 41  
                                        код( 36 бит )
       /   \
     /  \   73
   /  \  54  т
 / \  71  а  
13 15  р
-  *

BYTE | ai|ci | code_i - vector<char>
------------------
73   | т | 6 | 1
71   | р | 2 | 001
54   | а | 6 | 01
13   | - | 2 | 0000
15   | * | 1 | 0001


100101 0000 101101  0001   100101 0000 101101
 тра    -   тата     *      тра    -    тата

struct Node {
    Node* Left;
    Node* Right;
    unsigned char Byte;
    unsigned char Count;
};

Если встретили лист - пишем бит 1 и Byte из листа
Если встретили внутренний узел - пишем левую ветвь, пишем правую ветвь и бит 0
Дерево: 1-1*01р01а01т0 = 9 + 5*8 = 49 бит
Код: 36 бит

1-1*01р01а01т0

         /\
       /\  р
стек: -  * 

Итого: 85 бит
         5                   3                       1-1*01р01а01т0   100101 0000 101101  0001   100101 0000 101101 ? ? ?
(сколько символов + сколь незначимых битов в конце)  
*/

#include<vector>
using std::vector;

class OutBitStream {
    OutBitStream();

    void WriteBit( unsigned char bit );
    void WriteByte( unsigned char byte );

    const vector<unsigned char>& GetBuffer() const;
    size_t GetBitsCount() const;

private:
    vector<unsigned char> buffer;
    size_t bitsCount;
//  00 010101    01011101
};

OutBitStream::OutBitStream() : bitsCount( 0 )
{
}

void OutBitStream::WriteBit( unsigned char bit )
{
    if( bitsCount + 1 > buffer.size() * 8 ) {
        buffer.push_back( 0 );
    }

    if( bit != 0 ) {
        int bitPos = bitsCount % 8;
        buffer[bitsCount / 8] |= 1 << bitPos;
    }

    ++bitsCount;
}

void OutBitStream::WriteByte( unsigned char byte )
{
    if( bitsCount % 8 == 0 ) {
        buffer.push_back( byte );
    } else {
        int offset = bitsCount % 8;
        buffer[bitsCount / 8] |= byte << offset;
        buffer.push_back( byte >> (8 - offset) );
    }
    bitsCount += 8;
}

const vector<unsigned char>& OutBitStream::GetBuffer() const
{
    return buffer;
}

size_t OutBitStream::GetBitsCount() const
{
    return bitsCount;
}

#include<array>
#include<cassert>
#include<queue>
#include<iostream>
#include<string>
#include<unordered_map>

using std::array;
using std::queue;
using std::cout;
using std::ostream;
using std::swap;
using std::string;
using std::unordered_map;

const int SideSize = 3;
const int FieldSize = SideSize * SideSize;
const array<char, FieldSize> FinishField( { 1, 2, 3, 4, 5, 6, 7, 8, 0 } );

class GameState {
public:
	GameState( const array<char, FieldSize>& _field );

	bool CanMoveLeft() const;
	bool CanMoveUp() const;
	bool CanMoveRight() const;
	bool CanMoveDown() const;

	GameState MoveLeft() const;
	GameState MoveUp() const;
	GameState MoveRight() const;
	GameState MoveDown() const;

	bool IsFinish() const;

	char GetFieldValue( size_t pos ) const;

	bool operator == ( const GameState& state ) const { return field == state.field; }
	bool operator != ( const GameState& state ) const { return !(*this == state); }

private:
	array<char, FieldSize> field;
	size_t zeroPos;
};

GameState::GameState( const array<char, FieldSize>& _field ) :
	field( _field ),
	zeroPos( -1 )
{
	for( size_t i = 0; i < field.size(); ++i ) {
		if( field[i] == 0 ) {
			zeroPos = i;
			break;
		}
	}
	assert( zeroPos != -1 );
}

bool GameState::CanMoveLeft() const
{
	return zeroPos % SideSize != 0;
}

bool GameState::CanMoveUp() const
{
	return zeroPos >= SideSize;
}

bool GameState::CanMoveRight() const
{
	return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveDown() const
{
	return zeroPos < FieldSize - SideSize;
}

GameState GameState::MoveLeft() const
{
	assert( CanMoveLeft() );
	GameState newState( *this );
	swap( newState.field[zeroPos], newState.field[zeroPos - 1] );
	--newState.zeroPos;
	return newState;
}

GameState GameState::MoveUp() const
{
	assert( CanMoveUp() );
	GameState newState( *this );
	swap( newState.field[zeroPos], newState.field[zeroPos - SideSize] );
	newState.zeroPos -= SideSize;
	return newState;
}

GameState GameState::MoveRight() const
{
	assert( CanMoveRight() );
	GameState newState( *this );
	swap( newState.field[zeroPos], newState.field[zeroPos + 1] );
	++newState.zeroPos;
	return newState;
}

GameState GameState::MoveDown() const
{
	assert( CanMoveDown() );
	GameState newState( *this );
	swap( newState.field[zeroPos], newState.field[zeroPos + SideSize] );
	newState.zeroPos += SideSize;
	return newState;
}

bool GameState::IsFinish() const
{
	return field == FinishField;
}

char GameState::GetFieldValue( size_t pos ) const
{
	assert( pos >= 0 && pos < FieldSize );
	return field[pos];
}

struct StateHasher {
	size_t operator()( const GameState& state ) const {
		size_t hash = 0;
		size_t p = 1;
		for( int i = 0; i < FieldSize; ++i ) {
			p *= 137;
			hash += state.GetFieldValue( i ) * p;
		}
		return hash;
	}
};

string Get8thSolution( const GameState& state )
{
	queue<GameState> bfsQueue;
	bfsQueue.push( state );
	unordered_map<GameState, char, StateHasher> visited;
	visited[state] = 'S';
	while( bfsQueue.size() > 0 ) {
		GameState current = bfsQueue.front();
		bfsQueue.pop();
		if( current.IsFinish() ) {
			break;
		}
		if( current.CanMoveLeft() ) {
			GameState newState = current.MoveLeft();
			if( visited.find( newState ) == visited.end() ) {
				visited[newState] = 'L';
				bfsQueue.push( newState );
			}
		}
		if( current.CanMoveUp() ) {
			GameState newState = current.MoveUp();
			if( visited.find( newState ) == visited.end() ) {
				visited[newState] = 'U';
				bfsQueue.push( newState );
			}
		}
		if( current.CanMoveRight() ) {
			GameState newState = current.MoveRight();
			if( visited.find( newState ) == visited.end() ) {
				visited[newState] = 'R';
				bfsQueue.push( newState );
			}
		}
		if( current.CanMoveDown() ) {
			GameState newState = current.MoveDown();
			if( visited.find( newState ) == visited.end() ) {
				visited[newState] = 'D';
				bfsQueue.push( newState );
			}
		}
	}

	string result;
	GameState current( FinishField );
	char move = visited[current];
	while( move != 'S' ) {
		result += move;
		switch( move ) {
			case 'L':
				current = current.MoveRight();
				break;
			case 'U':
				current = current.MoveDown();
				break;
			case 'R':
				current = current.MoveLeft();
				break;
			case 'D':
				current = current.MoveUp();
				break;
		}
		move = visited[current];
	}

	std::reverse( result.begin(), result.end() );
	return result;
}

ostream& operator << ( ostream& out, const GameState& state )
{
	for( size_t y = 0; y < SideSize; ++y ) {
		for( size_t x = 0; x < SideSize; ++x ) {
			out << static_cast<int>( state.GetFieldValue( y * SideSize + x ) ) << " ";
		}
		out << std::endl;
	}
	out << std::endl;
	return out;
}

int main()
{
	GameState state( { 1, 5, 4, 8, 7, 3, 6, 2, 0 } );
	string result = Get8thSolution( state );
	for( char move : result ) {
		switch( move ) {
			case 'L':
				state = state.MoveLeft();
				break;
			case 'U':
				state = state.MoveUp();
				break;
			case 'R':
				state = state.MoveRight();
				break;
			case 'D':
				state = state.MoveDown();
				break;
			default:
				assert( false );
		}
		cout << state;
	}
	return 0;
}
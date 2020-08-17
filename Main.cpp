#include <Windows.h>
#include <stdio.h>
#include <string>

template< typename type1, typename type2 >
struct Tuple
{
	type1 element1;
	type2 element2;
};

Tuple<const char, const char*> characterTranslation[] = 
{
	//------ Numbers ------
	{ '0', /*=>*/ ":zero:" },
	{ '1', /*=>*/ ":one:"  },
	{ '2', /*=>*/ ":two:"  },
	{ '3', /*=>*/ ":three:"},
	{ '4', /*=>*/ ":four:" },
	{ '5', /*=>*/ ":five:" },
	{ '6', /*=>*/ ":six:"  },
	{ '7', /*=>*/ ":seven:"},
	{ '8', /*=>*/ ":eight:"},
	{ '9', /*=>*/ ":nine:" },
	//---------------------

	{ '#', /*=>*/ ":hash:" },
	{ '*', /*=>*/ ":asterisk:" },
	{ '!', /*=>*/ ":exclamation:" },
	{ '?', /*=>*/ ":question:" },

	{ '/', /*=>*/ "**÷**" },
	{ ':', /*=>*/ "**÷**" },
	{ '÷', /*=>*/ "**÷**" },

	{ '+', /*=>*/ "**+**" },
	{ '-', /*=>*/ "**-**" },
	{ '=', /*=>*/ "**=**" },
	{ ' ', /*=>*/ "     " },
};

static char letterTranslation[23] = ":regional_indicator_?:";

namespace Clipboard
{
	char* GetText( void );
	void SetText( const char* text );
};

void DiscordTranslateText( const char* text );
const char* translateCharacter( const char c );


int main( )
{
	char* ClipboardText = Clipboard::GetText();
	if ( ClipboardText )
	{
		DiscordTranslateText( ClipboardText );
		free( ClipboardText );
	}
	
	Sleep( 1000 );

	return 0;
}

const char* translateCharacter( const char c )
{
	if ( (c >= 'A' && c <= 'Z') || 
		 (c >= 'a' && c <= 'z') )
	{
		const char t = c
			
			//to lowercase
			+ ( (c < 'a') ? ' ' : '\0' );
		
		letterTranslation[ 20 ] = t;
		return letterTranslation;
	}
	
	for (size_t i = 0; i < ARRAYSIZE(characterTranslation); i++)
	{
		auto Element = &characterTranslation[ i ];
		if ( Element->element1 == c )
			return Element->element2;
	}
	return nullptr;
}

void DiscordTranslateText( const char* text )
{
	std::string result = "";

	size_t textlenght = strlen(text);

	bool lastCharacterIsColon = false;

	for (size_t i = 0; i < textlenght; i++)
	{
		const char c = text[i];
		//translate characters
		auto translation = translateCharacter( c );
		if ( translation != nullptr )
		{
			if ( lastCharacterIsColon == true )
				result += " ";

			result += translation;

			lastCharacterIsColon = true;
		}
		else
		{
			if ( lastCharacterIsColon == true )
				result += " ";

			lastCharacterIsColon = ( c == ':' );

			const char tempString[2] = { c , 0 };
			result += std::string( tempString );
		}
	}
	puts( result.c_str() );

	Clipboard::SetText( result.c_str() );
}

namespace Clipboard
{
	//function based on source code from: https://stackoverflow.com/a/14763025
	char* GetText( void )
	{
		// Try opening the clipboard
		if (OpenClipboard(nullptr) != TRUE)
			return nullptr;

		// Get handle of clipboard object for ANSI text
		HGLOBAL MemoryHandle = GetClipboardData(CF_TEXT);
		if (MemoryHandle == nullptr)
			return nullptr;

		// Lock the handle to get the actual text pointer
		const char* Data = static_cast<char*>( GlobalLock( MemoryHandle ) );
		if (Data == nullptr)
			return nullptr;

		char* Text = _strdup( Data );

		// Release the lock
		GlobalUnlock( MemoryHandle );

		// Release the clipboard
		CloseClipboard( );
		
		return Text;
	}

	//function based source code from: https://stackoverflow.com/a/1264179
	void SetText( const char* text )
	{
		const size_t TextLenght = strlen( text ) + 1;
		HGLOBAL MemoryHandle = GlobalAlloc(GMEM_MOVEABLE, TextLenght);
		if ( MemoryHandle == NULL )
			return;
		
		LPVOID AllocatedMemoryPointer = GlobalLock( MemoryHandle );
		memcpy( AllocatedMemoryPointer, text, TextLenght );

		GlobalUnlock( MemoryHandle );
		
		OpenClipboard( NULL );

		EmptyClipboard( );

		SetClipboardData( CF_TEXT, MemoryHandle );

		CloseClipboard( );

		GlobalFree( MemoryHandle );
	}
}

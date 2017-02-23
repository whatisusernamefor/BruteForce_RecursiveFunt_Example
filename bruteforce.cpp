#include <iostream>
#include <fstream>

  const char Version[] = "Version 0.0.1";
/*
Programmer: Joseph Stone
Gmail:      josephstonebusiness@gmail.com


	-Last Updated 24/02/17 04:12 - Version 0.0.1

*/


// Cheater Global Variables; 
// Not much reason to declare these here... 
std::size_t HAY_LEN = 0;      // smaller than 100
std::size_t LOOP_COUNTER = 0;
unsigned int MATCH_LEN = 0;   // smaller than 100
unsigned int PIN_COUNTER = 0; // smaller than 100 
unsigned char HAY_DELIM = 0x0A; 

    std::size_t SIZE = 2056;
    std::size_t pos = 0;
    unsigned char buff[300+1];
    unsigned int LEN_INPUT = 0;
    unsigned int COUNTER_S = 0;
    unsigned int NEAR_PTR = 0;
    unsigned char *dict;
/*
* ERR: 
*	1 == bad input
*	2 == bad format
*	
*	ALL FLAG == bad input 
*/
void recursiveAdd(unsigned char *, int index, const int &end, const int &numChars );
void addDict(unsigned char *&, std::size_t &, const unsigned char *, unsigned int & );
void printMenu(char **argv); 
int main( int argc, char **argv )  
{
    
    if( argc < 2 ) { printMenu(argv); return 1; }
    char tmp[] = "out.dat";
    char *outfile = 0; // tmp;
    char special_tmp[1]; special_tmp[0] = '\0';
    char *special = special_tmp; // special chars to create pass file
    char *alphaChars = 0;
    unsigned int maxStrlen = 0; 
    unsigned int minStrlen = 0;
    bool uppercase = false;

    for(unsigned int a=1, i=0, mult=1; a <= argc-1 && *(*(argv+a)+0)== '-' ; a++, i=0,mult=1) { 
	char c = *(*(argv+a)+1);
	if( c < 0x61 ) { c += 0x20; } // lower-case
	switch ( c ) { 
	
	    // OUTPUT FILE NAME
	    case 'o': 
		outfile = *(argv+a+1);
		a++; 
	    	break;
	   
	    // CUSTOM character array;
	    case 'c': 
		alphaChars = *(argv+a+1);
		a++; 
	    	break;

	    // UPPERCASE enabled? 
	    case 'u': 
		uppercase = true;
		a++; 
	    	break;

   	    // INPUT SPECIAL CHARACTERS, numbers, symbols;
	    case 's':
		special= *(argv+a+1);
		a++; 
		break;

            // maxlen of bruteforce;
	    case 'l':
		while( *(*(argv+a+1)+i) ) { i++; mult*=10;  }
		i=0; mult/=10;
		while( *(*(argv+a+1)+i) ) { 
		    maxStrlen+= (*(*(argv+a+1)+i)-0x30)*mult;
		    i++; mult/=10; 
		}
		a++;
	        break;

            // minlen of bruteforce;
	    case 'm':
		while( *(*(argv+a+1)+i) ) { i++; mult*=10;  }
		i=0; mult/=10;
		while( *(*(argv+a+1)+i) ) { 
		    minStrlen+= (*(*(argv+a+1)+i)-0x30)*mult;
		    i++; mult/=10; 
		}
		a++;
	        break;
			
		case 'v':
		std::cout << Version << std::endl; 
		return 0;
	}
    }   

    if( maxStrlen > 12 || maxStrlen == 0 ) { return 0xFF; }
    unsigned int len = 0;
    while( special[len] ) { len++; }
    len+= 26;
    if( alphaChars ) { len-=26; for(int i=0;alphaChars[ i ];i++) { len++; } }
    unsigned int inputSize = len;
    len = 0;
    
    unsigned char bruteforce[ inputSize+1 ]; 

    if( !alphaChars ) { 
    	while( len < 26 ) { bruteforce[ len ] = len++; };    
    } else { 
	    while( alphaChars[ len ] ) { bruteforce[ len ] = alphaChars[ len ];  len++; } 
    }
    for(int i=0; special[ i ]; i++, len++) { bruteforce[ len ] = special[i]; };    
    bruteforce[len]= '\0';

    unsigned char strCount[ maxStrlen+1 ];
    for(int i=0; i < maxStrlen; i++) { strCount[ i ] = 0; }
std::cout <<"("<< bruteforce <<")"<< std :: endl;

    std::size_t LOG_SIZE = 0; // holds pos of index in dict;
    SIZE = 2056; // ALREADY DEFINED!
    pos = 0;      // LAREADY DEFINED!

    dict = new unsigned char[SIZE+1]; dict[0] = 0x0A; dict[1]='\0';
    // char buff[len+1];    
    std::size_t numIterations = 1;
    for(int i=0; i < maxStrlen; i++) { numIterations*= inputSize; }
//std::cout << numIterations<<std::endl;

    unsigned char buff[ inputSize+1 ];
    int z= 0;
    for(std::size_t i=0; i < numIterations; i++) { 
//std::cout << "("<<i<<"); ";        
  
        // DICT CHECK!
	    // len++;  now becomes buffLen... for addDict! 
	    // PASTED ABOVE; addDict( dict, pos, buff, buffIndex );
	    if( LOG_SIZE+maxStrlen+1 >= SIZE ) { 
		unsigned char * t = dict;
		dict = new unsigned char[ SIZE+SIZE+1 ];
		LOOP_COUNTER = 0;
		while( LOOP_COUNTER <= LOG_SIZE ) { dict[ LOOP_COUNTER ] = t[ LOOP_COUNTER ]; LOOP_COUNTER++; } // [POS] == DELIM ( 0x0A )
		delete [] t; SIZE+= SIZE; 
	    }
        
        z=0;while( z<maxStrlen ){ buff[ z ] = bruteforce[ strCount[ z ] ]; z++; }
        addDict(dict, LOG_SIZE, buff, maxStrlen );
        recursiveAdd(strCount, 0, maxStrlen, inputSize );
 
    }


    if( !outfile ) { 
        std::cout << dict; 
    } 
    else {  
        std::ofstream out; out.open( outfile, std::ios::binary );
    
        out << "Special Chars: "<< special << std::endl
            << "alphaChars:    "<< alphaChars<< std::endl 
            << dict;
    }
    return 0;
}

void recursiveAdd(unsigned char *strCount, int index, const int &end, const int &numChars ) {
    if( index == end ) { return ; }

    if( strCount[ index ] == numChars-1 ) { 
        strCount[ index ] = 0;
        recursiveAdd( strCount, index+1, end, numChars );
    }
    else { 
        strCount[ index ]++; 
    }
    
    return;
}
void addDict(unsigned char *&dict, 	std::size_t &HAY_SIZE, 
	     const unsigned char *pin, 	unsigned int &PIN_SIZE ){
// std::size_t PIN_SIZE = PIN_SIZEE;

if( PIN_SIZE == 0 ) { std::cout << "== 0 \n"; return; }

/*
    LOOP_COUNTER = 0;
    while( LOOP_COUNTER+PIN_SIZE <= HAY_SIZE ) { 	
    	HAY_LEN = 0;
    	// 	Grab an individual entry from HAYSTACK; GET ITS LENGTH; 
    	while( 
LOOP_COUNTER+HAY_LEN < HAY_SIZE &&
        dict[ LOOP_COUNTER+HAY_LEN ] != HAY_DELIM ) { HAY_LEN++; }

    	if( HAY_LEN == PIN_SIZE ) { 
    	    MATCH_LEN = 0;
    	    while( MATCH_LEN<HAY_LEN&& dict[ LOOP_COUNTER+MATCH_LEN ] == pin[ MATCH_LEN ] ) {
                    MATCH_LEN++; }
    	
      	    if( MATCH_LEN == HAY_LEN ) { return ; }
    	}
    
    	LOOP_COUNTER+= HAY_LEN+1;
    }
*/

    LOOP_COUNTER = 0; 
    std::size_t  plusOne = 1;
    if( !HAY_SIZE ) { plusOne = 0; }// used to increase HAY_SIZE, sub for all else!
    //  +1 NOT OVERWRITING the NULL_TERMINAOTR! Adda nother at end of line;
    while(

     LOOP_COUNTER < PIN_SIZE ) { 
	    dict[ HAY_SIZE+plusOne+LOOP_COUNTER ] = pin[ LOOP_COUNTER ];
	    LOOP_COUNTER++;
	}
    HAY_SIZE+= PIN_SIZE +plusOne;
    dict[ HAY_SIZE ] = HAY_DELIM; 
    dict[ HAY_SIZE+1 ] = '\0'; 

    return ; 
}

void printMenu(char **argv) { 
        std::cout << "Creates custom bruteforce file using user's input.\n\n"
         << *(argv+0)<<" [ -C string ] [ -U ] [ -l MAX_LENGTH ] \n"
		 << "[ -m MIN_LENGTH ] [-O OutputFile] [ -v ] \n"
         << "\n"
		<< "  -C\tCreates output file with characters supplied as 'string.'\n"
		<< "  -U\t**Creates output file with lower and upper case of 'string.'\n" 
		<< "  -L\tMax length of characters the output file will contain.\n"
		<< "  -M\tMinimum length of characters the output file will contain.\n"
		<< "  -O\tSpecifies the output filename. [ Appends to file if exists. ]\n"
		<< "  -V\tPrints the version of this application.\n"
		<< "\n"
        << "  **\tNot Implemented!\n";
}

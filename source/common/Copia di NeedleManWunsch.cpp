#include "stdafx.h"
#include "NeedlemanWunsch.h"

int NeedlemanWunsch::Blosum62[24][24] = {
/* A Ala 0 */  {4,-1,-2,-2,0,-1,-1,0,-2,-1,-1,-1,-1,-2,-1,1,0,-3,-2,0,-2,-1,0,-4},
/* R Arg 1 */  {-1,5,0,-2,-3,1,0,-2,0,-3,-2,2,-1,-3,-2,-1,-1,-3,-2,-3,-1,0,-1,-4},
/* N Asn 2 */  {-2,0,6,1,-3,0,0,0,1,-3,-3,0,-2,-3,-2,1,0,-4,-2,-3,3,0,-1,-4},
/* D Asp 3 */  {-2,-2,1,6,-3,0,2,-1,-1,-3,-4,-1,-3,-3,-1,0,-1,-4,-3,-3,4,1,-1,-4},
/* C Cys 4 */  {0,-3,-3,-3,9,-3,-4,-3,-3,-1,-1,-3,-1,-2,-3,-1,-1,-2,-2,-1,-3,-3,-2,-4},
/* Q Gln 5 */  {-1,1,0,0,-3,5,2,-2,0,-3,-2,1,0,-3,-1,0,-1,-2,-1,-2,0,3,-1,-4},
/* E Glu 6 */  {-1,0,0,2,-4,2,5,-2,0,-3,-3,1,-2,-3,-1,0,-1,-3,-2,-2,1,4,-1,-4},
/* G Gly 7 */  {0,-2,0,-1,-3,-2,-2,6,-2,-4,-4,-2,-3,-3,-2,0,-2,-2,-3,-3,-1,-2,-1,-4},
/* H His 8 */  {-2,0,1,-1,-3,0,0,-2,8,-3,-3,-1,-2,-1,-2,-1,-2,-2,2,-3,0,0,-1,-4},
/* I Ile 9 */  {-1,-3,-3,-3,-1,-3,-3,-4,-3,4,2,-3,1,0,-3,-2,-1,-3,-1,3,-3,-3,-1,-4},
/* L Leu 10 */ {-1,-2,-3,-4,-1,-2,-3,-4,-3,2,4,-2,2,0,-3,-2,-1,-2,-1,1,-4,-3,-1,-4},
/* K Lys 11 */ {-1,2,0,-1,-3,1,1,-2,-1,-3,-2,5,-1,-3,-1,0,-1,-3,-2,-2,0,1,-1,-4},
/* M Met 12 */ {-1,-1,-2,-3,-1,0,-2,-3,-2,1,2,-1,5,0,-2,-1,-1,-1,-1,1,-3,-1,-1,-4},
/* F Phe 13 */ {-2,-3,-3,-3,-2,-3,-3,-3,-1,0,0,-3,0,6,-4,-2,-2,1,3,-1,-3,-3,-1,-4},
/* P Pro 14 */ {-1,-2,-2,-1,-3,-1,-1,-2,-2,-3,-3,-1,-2,-4,7,-1,-1,-4,-3,-2,-2,-1,-2,-4},
/* S Ser 15 */ {1,-1,1,0,-1,0,0,0,-1,-2,-2,0,-1,-2,-1,4,1,-3,-2,-2,0,0,0,-4},
/* T Thr 16 */ {0,-1,0,-1,-1,-1,-1,-2,-2,-1,-1,-1,-1,-2,-1,1,5,-2,-2,0,-1,-1,0,-4},
/* W Trp 17 */ {-3,-3,-4,-4,-2,-2,-3,-2,-2,-3,-2,-3,-1,1,-4,-3,-2,11,2,-3,-4,-3,-2,-4},
/* Y Tyr 18 */ {-2,-2,-2,-3,-2,-1,-2,-3,2,-1,-1,-2,-1,3,-3,-2,-2,2,7,-1,-3,-2,-1,-4},
/* V Val 19 */ {0,-3,-3,-3,-1,-2,-2,-3,-3,3,1,-2,1,-1,-2,-2,0,-3,-1,4,-3,-2,-1,-4},
/* B */        {-2,-1,3,4,-3,0,1,-1,0,-3,-4,0,-3,-3,-2,0,-1,-4,-3,-3,4,1,-1,-4},
/* Z, */       {-1,0,0,1,-3,3,4,-2,0,-3,-3,1,-1,-3,-1,0,-1,-3,-2,-2,1,4,-1,-4},
/* X,*/        {0,-1,-1,-1,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2,0,0,-2,-1,-1,-1,-1,-1,-4},
/* *, */       {-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,1}
	};





using namespace std;

NeedlemanWunsch::NeedlemanWunsch(bool ProteinOrNucleotide, // true = protein, false = Nucleotide
					CString & Seq1,
					CString & Seq2){
	seq_1 = Seq1;
	seq_2 = Seq2;

	Aligned = false;
}

double NeedlemanWunsch::Align(){ // result is the score

	return 0;
}


NeedlemanWunsch::~NeedlemanWunsch(){
}

int NeedlemanWunsch::nw(                                                          
//        string       seq_1,          /*  Needleman-Wunsch   */
  //      string       seq_2,          /*  algorithm for      */
//        string&      seq_1_al,       /*  global alignment   */
//        string&      seq_2_al,       /*  of nt sequence.    */
//        bool         prm
      )
{
        int  d = 2 ;                 /* gap penalty */
        d = 4 ;                 /* gap penalty */

        int  L1 = seq_1.GetLength();
        int  L2 = seq_2.GetLength();

        // Dynamic programming matrix
        int ** F = new int * [ L2+1 ];
        for( int i = 0; i <= L2; i++ )  F[ i ] = new int [ L1 ];

        // Traceback matrix
        char ** traceback = new char * [ L2+1 ];
        for( int i = 0; i <= L2; i++ )  traceback[ i ] = new char [ L1 ];

        // Initialize traceback and F matrix (fill in first row and column)
        dpm_init( F, traceback, L1, L2, d );

        // Create alignment
        nw_align( F, traceback, seq_1, seq_2, seq_1_al, seq_2_al, d );

/*
#if DEBUG
            int  L_al = seq_1_al.length();
            cout << "Length after alignment: " << L_al << endl;
        #endif
*/
        if( false )
        {
//                cout << "\nDynamic programming matrix: " << "\n\n";
                print_matrix( F, seq_1, seq_2 );

 //               cout << "\nTraceback matrix: " << "\n\n";
                print_traceback( traceback, seq_1, seq_2 );

//                cout << endl;
        }

        for( int i = 0; i <= L2; i++ )  delete F[ i ];
        delete[] F;
        for( int i = 0; i <= L2; i++ )  delete traceback[ i ];
        delete[] traceback;

        return  0 ;
}


void  NeedlemanWunsch::dpm_init( int ** F, char ** traceback, int L1, int L2, int d )
{
        F[ 0 ][ 0 ] =  0 ;
        traceback[ 0 ][ 0 ] = 'n' ;

        int i=0, j=0;

        for( j = 1; j <= L1; j++ )
        {
                F[ 0 ][ j ] =  -j * d ;
                traceback[ 0 ][ j ] =  '-' ;
        }
        for( i = 1; i <= L2; i++ )
        {
                F[ i ][ 0 ] =  -i * d ;
                traceback[ i ][ 0 ] =  '|' ;
        }
}


int NeedlemanWunsch::nw_align(                  // Needleman-Wunsch algorithm
              int **     F,
              char **    traceback,
              CString     seq_1,
              CString     seq_2,
              CString&    seq_1_al,
              CString&    seq_2_al,
              int        d         // Gap penalty
            )
{
        int        k = 0, x = 0, y = 0;
        int        fU, fD, fL ;
        char       ptr, nuc ;
        int        i = 0, j = 0;

/*
const int  a =  2;   // Match
        const int  b = -1;   // Mismatch

        const int  s[ 4 ][ 4 ] = { { a, b, b, b },
                                   { b, a, b, b },
                                   { b, b, a, b },
                                   { b, b, b, a } } ;
*/

        int  L1 = seq_1.GetLength();
        int  L2 = seq_2.GetLength();

        for( i = 1; i <= L2; i++ )
        {
                for( j = 1; j <= L1; j++ )
                {
                        nuc = seq_1[ j-1 ] ;
						x = GetResidueIndexFromSingleLetterCode(nuc);

/*                        switch( nuc )
                        {
                                case 'A':  x = 0 ;  break ;
                                case 'C':  x = 1 ;  break ;
                                case 'G':  x = 2 ;  break ;
                                case 'T':  x = 3 ;
                        }
*/
                        nuc = seq_2[ i-1 ] ;
						y = GetResidueIndexFromSingleLetterCode(nuc);

/*
switch( nuc )
                        {
                                case 'A':  y = 0 ;  break ;
                                case 'C':  y = 1 ;  break ;
                                case 'G':  y = 2 ;  break ;
                                case 'T':  y = 3 ;
                        }
*/

                        fU = F[ i-1 ][ j ] - d ;
//                        fD = F[ i-1 ][ j-1 ] + s[ x ][ y ] ;
                        fD = F[ i-1 ][ j-1 ] + Blosum62[ x ][ y ] ;
                        fL = F[ i ][ j-1 ] - d ;

                        F[ i ][ j ] = NWmax( fU, fD, fL, &ptr ) ;

                        traceback[ i ][ j ] =  ptr ;
                }
        }
        i-- ; j-- ;

        while( i > 0 || j > 0 )
        {
                switch( traceback[ i ][ j ] )
                {
                        case '|' :      seq_1_al += '-' ; 
                                        seq_2_al += seq_2[ i-1 ] ; 
                                        i-- ;
                                        break ;

                        case '\\':      seq_1_al += seq_1[ j-1 ] ; 
                                        seq_2_al += seq_2[ i-1 ] ; 
                                        i-- ;  j-- ;
                                        break ;

                        case '-' :      seq_1_al += seq_1[ j-1 ] ; 
                                        seq_2_al += '-' ; 
                                        j-- ;
                }
                k++ ;
        }
 

 //       reverse( seq_1_al.begin(), seq_1_al.end() );
   //     reverse( seq_2_al.begin(), seq_2_al.end() );

        return  0 ;
}


int  NeedlemanWunsch::NWmax( int f1, int f2, int f3, char * ptr )
{
        int  max = 0 ;

        if( f1 >= f2 && f1 >= f3 )  
        {
                max = f1 ;
                *ptr = '|' ;
        }
        else if( f2 > f3 )              
        {
                max = f2 ;
                *ptr = '\\' ;
        }
        else
        {
                max = f3 ;
                *ptr = '-' ;
        }
        
        return  max ;   
}


void  NeedlemanWunsch::print_matrix( int ** F, CString seq_1, CString seq_2 )
{
        int  L1 = seq_1.GetLength();
        int  L2 = seq_2.GetLength();

//        cout << "        ";
        for( int j = 0; j < L1; j++ )
        {
//                cout << seq_1[ j ] << "   ";
        }
  //      cout << "\n  ";

        for( int i = 0; i <= L2; i++ )
        {
                if( i > 0 )
                {
  //                      cout << seq_2[ i-1 ] << " ";
                }
                for( int j = 0; j <= L1; j++ )
                {
  //                      cout.width( 3 );
    //                    cout << F[ i ][ j ] << " ";
                }
      //          cout << endl;
        }
}


void  NeedlemanWunsch::print_traceback( char ** traceback, CString seq_1, CString seq_2 )
{
        int  L1 = seq_1.GetLength();
        int  L2 = seq_2.GetLength();

//        cout << "    ";
        for( int j = 0; j < L1; j++ )
        {
  //              cout << seq_1[ j ] << " ";
        }
  //      cout << "\n  ";

        for( int i = 0; i <= L2; i++ )
        {
                if( i > 0 )
                {
    //                    cout << seq_2[ i-1 ] << " ";
                }
                for( int j = 0; j <= L1; j++ )
                {
      //                  cout << traceback[ i ][ j ] << " ";
                }
     //           cout << endl;
        }
}


void  NeedlemanWunsch::print_al( CString& seq_1_al, CString& seq_2_al )
{
      //  cout << seq_1_al << endl;
      //  cout << seq_2_al << endl;
}



int NeedlemanWunsch::GetResidueIndexFromSingleLetterCode(char res){
	switch( res )
		{
			case 'A' : return 0;break ;
			case 'R' : return 1;break ;
			case 'N' : return 2;break ;
			case 'D' : return 3;break ;
			case 'C' : return 4;break ;
			case 'Q' : return 5;break ;
			case 'E' : return 6;break ;
			case 'G' : return 7;break ;
			case 'H' : return 8;break ;
			case 'I' : return 9;break ;
			case 'L' : return 10;break ;
			case 'K' : return 11;break ;
			case 'M' : return 12;break ;
			case 'F' : return 13;break ;
			case 'P' : return 14;break ;
			case 'S' : return 15;break ;
			case 'T' : return 16;break ;
			case 'W' : return 17;break ;
			case 'Y' : return 18;break ;
			case 'V' : return 19;break ;
			default: return 23;break ;
	}
}



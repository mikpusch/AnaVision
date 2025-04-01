#include "stdafx.h"
#include "NeedlemanWunsch.h"

//#include <string.h>



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

	seq_1.MakeUpper();
	seq_2.MakeUpper();

	gap_open = GapOpenDefault();
	gap_extend = GapExtendDefault();

	Aligned = false;
}

int NeedlemanWunsch::GapOpenDefault(){
	return -8;
}
int NeedlemanWunsch::GapExtendDefault(){
	return -2;
}

double NeedlemanWunsch::Align(CString & AlignString, bool MakeAlignstrings,
							  CString & StringA, CString & StringB, CString & StringM){ // result is the score

	double result = DoAlign(seq_1, seq_2, gap_open, gap_extend, AlignString, MakeAlignstrings,
		StringA, StringB, StringM);

	return result;
}


NeedlemanWunsch::~NeedlemanWunsch(){
}

double NeedlemanWunsch::DoAlign(CString f1, CString f2,  int gap_open, int gap_extn, 
								CString & AlignString,
								bool MakeAlignString,
								CString & cA, CString & cB, CString & cM){

		int imut[24][24];      
		for (int i=1; i<24; i++){
			for (int j=1; j<24; j++){
				imut[i][j]=Blosum62[i-1][j-1];
			}
		}
//		Blosum62Matrix(imut);                              // Read Blosum scoring matrix and store it in the imut variable.
		CString seqW = "*ARNDCQEGHILKMFPSTWYVBZX";	       // Amino acide order in the BLAST's scoring matrix (e.g.,Blosum62). 
		f1 = "*" + f1;                                     // Add a '*' character in the head of a sequence and this can make java code much more consistent with orginal fortran code.   
		f2 = "*" + f2;                                     // Use 1 to represent the first position of the sequence in the original fortran code,and 1 stand for the second position in java code. Here, add a '*' character in the head of a sequence could make 1 standard for the first postion of thse sequence in java code.     
		int L1 = f1.GetLength();
		int L2 = f2.GetLength();
//		ShowFloat(L1, "L1");
//		ShowFloat(L2, "L2");
		int * seq1 = new int[L1+1];                 
		int * seq2 = new int[L2+1];		           // seq1 and seq2 are arrays that store the amino acid order numbers of sequence1 and sequence2.
		
		// For example, 1 stand for A, 2 represent R and etc.
		for (int i=1;i<L1;i++)
		{
			bool found=false;
			for (int j=1;j<seqW.GetLength();j++)
			{
//				if(f1.charAt(i)==seqW.charAt(j))
				if (f1[i]==seqW[j])	{
					seq1[i]=j;
					found = true;
//					ShowFloat(j, "1 j");
				}
			}
			if (!found){
				Alert0(CString(f1[i]));
			}

		}		
		
		for (int i=1;i<L2;i++)
		{
			bool found=false;
			for (int j=1;j<seqW.GetLength();j++)
			{
				if (f2[i]==seqW[j])
				{
					seq2[i]=j;
					found = true;
				}
			}
			if (!found){
				Alert0(CString(f2[i]));
			}
		}
		
	 	 int** score = imatrix(0, L1, 0, L2);		// score[i][j] stard for the alignment score that align ith position of the first sequence to the jth position of the second sequence.
		 for (int i=1;i<L1;i++)
		 {
			  for (int j=1;j<L2;j++)
			  {
				  score[i][j] = imut[seq1[i]][seq2[j]];
			  }
		 }	
		
		int * j2i = new int[L2+2];
		for( int j=1;j<L2;j++)
		{
			j2i[j] = -1; // !all are not aligned
		}		
		
		int ** val = imatrix(0, L1+1, 0, L2+1);  // val[][] was assigned as a global variable, and the value could be printed in the final.
		int ** idir = imatrix(0, L1+1, 0, L2+1);
		int ** preV = imatrix(0, L1+1, 0, L2+1);
		int ** preH = imatrix(0, L1+1, 0, L2+1);
		int D,V,H;
		bool standard = true;
	//	int i, j;	
//		if(standard)    // If you want to use alternative implementation of Needleman-Wunsch dynamic program , you can assign "false" value to the "standard" variable.  
//		{			
			////////////////////////////////////////////////////////////////////////////////
			//	     This is a standard Needleman-Wunsch dynamic program (by Y. Zhang 2005).
			//	     1. Count multiple-gap.
			//	     2. The gap penality W(k)=Go+Ge*k1+Go+Ge*k2 if gap open on both sequences			
			//	     idir[i][j]=1,2,3, from diagonal, horizontal, vertical
			//	     val[i][j] is the cumulative score of (i,j)
			////////////////////////////////////////////////////////////////////////////////			
			
			int ** jpV = imatrix(0, L1+1, 0, L2+1);; //new int[L1+1][f2.GetLength()+1];
			int ** jpH = imatrix(0, L1+1, 0, L2+1); //new int[f1.GetLength()+1][f2.GetLength()+1];	
			for (int i=0; i<L1; i++){
				for (int j=0; j<L2; j++){
					jpH[i][j]=i*j;
				}
			}
			val[0][0] = 0;		
			for (int ii=1;ii<L1;ii++)
			{
				val[ii][0] = gap_extn*ii;
				preV[ii][0] = val[ii][0]; // not use preV at the beginning
		        idir[ii][0] = 0;         // useless
		        jpV[ii][0] = 1;          // useless
		        jpH[ii][0] = ii;          // useless
			}
			
			for (int jj=1;jj<L2;jj++)
			{
				 val[0][jj] = gap_extn*jj;
		         preH[0][jj] = val[0][jj];
		         idir[0][jj] = 0;
		         jpV[0][jj] = jj;
		         jpH[0][jj] = 1;
			}
			
			// DP ------------------------------>
			for (int j=1;j<L2;j++)
			{			
				for (int i=1;i<L1;i++)
				{			
					// D=VAL(i-1,j-1)+SCORE(i,j)--------------->
					D = val[i-1][j-1] + score[i][j];	// from diagonal, val(i,j) is val(i-1,j-1)			
					
					//	H=H+gap_open ------->	
	//				ShowFloat(i, "i");
	//				ShowFloat(j, "j");
					jpH[i][j] = 1;				
					int val1 = val[i-1][j] + gap_open;  // gap_open from both D and V
					int val2 = preH[i-1][j] + gap_extn; // gap_extn from horizontal
					if (val1>val2)   // last step from D or V
					{
						H = val1;
					}
					else            // last step from H
					{
						H = val2;
						if (i > 1)
						{
							jpH[i][j] = jpH[i-1][j] + 1;  // record long-gap
						}
					}
					
					//	V=V+gap_open --------->					
					jpV[i][j] = 1;
					val1 = val[i][j-1] + gap_open;
					val2 = preV[i][j-1] + gap_extn;
					if (val1>val2)
					{
						V = val1;
					}
					else
					{
						V = val2;
						if (j > 1)
						{
							jpV[i][j] = jpV[i][j-1] + 1;   // record long-gap   
						}
					}
					
					preH[i][j] = H; // unaccepted H
					preV[i][j] = V;	// unaccepted V				
					if ((D>H)&&(D>V))
					{
						idir[i][j]=1;
						val[i][j]=D;
					}
					else if (H > V)
					{   
						idir[i][j] = 2;
		                val[i][j] = H;
					}
		            else
		            {
		            	 idir[i][j] = 3;
			              val[i][j] = V;
		            }
				}			

			}			
		
			//  tracing back the pathway			

			int i = L1-1;
			int j = L2-1;		
			while ((i>0)&&(j>0))  
			{			 
				 if (idir[i][j]==1)       // from diagonal
				 {
					j2i[j] = i;
		            i=i-1;
		            j=j-1;
				 }
				 else if (idir[i][j]==2)  // from horizonal
		         { 	        	 
					 int temp1 = jpH[i][j];                  //  
		        	 for (int me=1; me<=temp1; me++)            //  In the point view of a programer, 
		        	 {                                       //  you should not use the  "for(int me=1;me<=jpH[i][j];me++)".
		        		if (i>0)	        	                 //  If you use up sentence,the value of jpH[i][j] is changed when variable i changes. 
		                {                                    //  So the value of jpH[i][j] was assigned to the value temp1 and use the setence "for(int me=1;me<=temp1;me++)" here. 
		            	   i=i-1;                            // 
		                }	                                 //
		        	  }	        	                                
		         }
				 else
		         {	 
					int temp2 = jpV[i][j]; 
		            for ( int me=1; me<=temp2; me++)             //  In the point view of a programer,
		            {                                        //  you should not use the  "for(int me=1;me<=jpV[i][j];me++)".
		               if (j>0)                               //  Because when variable i change, the jpV[i][j] employed here is also change. 
		               {                                     //  So the value of jpV[i][j] was assigned to the value temp2 and use the setence "for(int me=1;me<=temp2;me++)" here.
		                  j=j-1;                             //
		               }
		            }	           
		         }			 
			}	
//		}
		
		// calculate sequence identity			
		int L_id = 0;
	    int L_ali = 0;
	    for (int j=1;j<L2;j++) {	    		
	    		if (j2i[j]>0) {
	    			i=j2i[j];
	    			L_ali=L_ali+1;
		            if (seq1[i]==seq2[j]) {	            	
		            	L_id=L_id+1;
		            }
	            } 	        
	    }	   
	    
	    double identity = L_id*1.0/(L2-1);	    
	    int fina_score = val[L1-1][L2-1];
//		System.out.println("Alignment score=" + fina_score);
//	    System.out.println("Length of sequence 1:" + (f1.GetLength()-1));
//	    System.out.println("Length of sequence 2:" + (f2.GetLength()-1));
//	    System.out.println("Aligned length      :" + L_ali);
//	    System.out.println("Identical length    :" + L_id);
	    
//	    DecimalFormat df = new DecimalFormat("0.000");      // Correct the identity to 3 decimal places. 
//	    float df = new DecimalFormat("0.000");      // Correct the identity to 3 decimal places. 
//	    System.out.print("Sequence identity=" + df.format(identity));
//	    System.out.println(" " + L_id  + "/" + (f2.GetLength()-1));		    
//	    System.out.println();
	    
	    // output aligned sequences	    
		if (MakeAlignString){
		    char * sequenceA = new char[L1+L2];
		    char * sequenceB = new char[L1+L2];
			char * sequenceM = new char[L1+L2];	    
		    int k = 0;
		    i=1;
			j=1;	    
		    while (true)
		    {	    	
				if ((i>(L1-2))&&(j>(L2-2))){
	    			break;	    	
				}
		    	if ((i>(L1-1))&&(j<(L2-1)))     // unaligned C on 1
			    {
			    	k = k + 1;
					sequenceA[k] = '-';
		    		sequenceB[k] = seqW[seq2[j]];
		    		sequenceM[k] = ' ';
					j = j + 1;
			    }	    	
				else {
					if ((i<(L1-1))&&(j>(L2-1))) // unaligned C on 2
				    {	        	
						k = k + 1;
			    		sequenceA[k] = seqW[seq1[i]];
				    	sequenceB[k] = '-';
						sequenceM[k] = ' ';
			    		i = i + 1;
					}
					else{
						if (i==j2i[j]) // if align
				        {
							k = k + 1;
							sequenceA[k] = seqW[seq1[i]];
		        			sequenceB[k] = seqW[seq2[j]];
				     		if (seq1[i]==seq2[j])  // identical
					    		{
								sequenceM[k] = ':';
					    		}
						   	else
							{
								sequenceM[k] = ' ';
				    		}
					       	i = i + 1;
				        	j = j + 1;
						}
						else{
							if (j2i[j]<0)   // gap on 1
								{
		     					k = k + 1;
								if (seq2[j]<0){
//									ShowFloat(j, "j");
//									ShowFloat(L2, "L2");
//									ShowFloat(seq2[j], "seq2[j]");
//										ShowFloat(i, "i");
//										ShowFloat(seq1[i], "seq1[i]");
//										ShowFloat(L1, "L1");
								}
								if (seq2[j]>20){
//									ShowFloat(j, "j");
//									ShowFloat(L2, "L2");
//									ShowFloat(seq2[j], "seq2[j]");
//									ShowFloat(L2, "L2");
								}
						     	sequenceA[k] = '-';
							   	sequenceB[k] = seqW[seq2[j]];
							 	sequenceM[k] = ' ';
						 		j = j + 1;
							}   
							else{
								if (j2i[j] >= 0)  // gap on 2
									{
							     	k = k + 1;
									if (seq1[i]<0){
//										ShowFloat(i, "i");
//										ShowFloat(seq1[i], "seq1[i]");
									}
									if (seq1[i]>20){
//										ShowFloat(i, "i");
//										ShowFloat(seq1[i], "seq1[i]");
									}
							     	sequenceA[k] = seqW[seq1[i]];
							     	sequenceB[k] = '-';
								 	sequenceM[k] = ' ';		
						     		i = i + 1;
								 }
						    }   
						}
					}
				}
			}
//		    for (int i=1;i<=k;i++)
//		    {
//	    	System.out.print(sequenceA[i]);		   
//		    }
	//	    System.out.println();
//		    for (int i=1;i<=k;i++)
//		    {
//	    	System.out.print(sequenceM[i]);		   
//		    }
//	    System.out.println();
//		    for (int i=1;i<=k;i++)
//		    {
//	    	System.out.print(sequenceB[i]);		   
//		    }
//	    System.out.println();	
//		    for (int i=1;i<=k;i++)
//		    {
//		    	int temp = i%9;
//	    	System.out.print(temp);		   
//		    }
//	    System.out.println();   
	
			for (int i=1; i<=k; i++){
				AlignString += sequenceA[i];
				cA += sequenceA[i];
			}
//			AlignString += "\r\n";
			for (int i=1; i<=k; i++){
				AlignString += sequenceB[i];
				cB += sequenceB[i];
			}
//			AlignString += "\r\n";
			for (int i=1; i<=k; i++){
				AlignString += sequenceM[i];
				cM += sequenceM[i];
			}

//			Alert0(AlignString);
//		Alert0(sequenceB);
//		Alert0(sequenceM);
//			sequenceA[k+1]=0;
//			sequenceB[k+1]=0;
			for (int i=1; i<=k; i++){
				AlignString += sequenceM[i];
			}

		    delete [] sequenceM;
		    delete [] sequenceB;
			delete [] sequenceA;
		}

		free_imatrix(jpH, 0, L1+1, 0, L2+1); //new int[f1.GetLength()+1][f2.GetLength()+1];								
		free_imatrix(jpV, 0, L1+1, 0, L2+1);; //new int[L1+1][f2.GetLength()+1];
	

		free_imatrix(preH, 0, L1+1, 0, L2+1);
		free_imatrix(preV, 0, L1+1, 0, L2+1);
		free_imatrix(idir, 0, L1+1, 0, L2+1);
		free_imatrix(val, 0, L1+1, 0, L2+1);

		delete [] j2i;
	 	free_imatrix(score, 0, L1, 0, L2);	

		delete [] seq2;
		delete [] seq1;

		return fina_score;
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



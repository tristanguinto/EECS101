#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS 500
#define COLS 500
#define midx 250
#define midy 250


void clear( unsigned char image[ROWS][COLS] );
void header( int row, int col, unsigned char head[32] );



int main(void){

	int i,j;
	FILE *fp;
	
	unsigned char image[ROWS][COLS],head[32];

	char			filename[50];
	header(ROWS,COLS,head);
	strcpy ( filename, "image9.ras" );
	
	double a,m,alpha,hn;
	double x,y,r,L,L_l,L_s,am;
	double nds,hdn;
	double s[3],h[3],v[3],n[3];


	clear(image);
	//configuration 1 
	//s = [0,0,1] r = 50 a = 0.5 m = 1

/*
	r = 50;
	a = 0.5;
	m = 1;
	s[0] = 0;
	s[1] = 0;
	s[2] = 1;
*/

	//configuration 2 
	//s = [1/rad3,1/rad3,1/rad3] r = 50 a = 0.5 m = 1

/*
	r = 50;
	a = 0.5;
	m = 1;
	s[0] = 1/sqrt(3);
	s[1] = 1/sqrt(3);
	s[2] = 1/sqrt(3);

*/
	//configuration 3 
	//s = [1,0,0] r = 50 a = 0.5 m = 1

/*
	r = 50;
	a = 0.5;
	m = 1;
	s[0] = 1;
	s[1] = 0;
	s[2] = 0;
*/

	//configuration 4 
	//s = [0,0,1] r = 10 a = 0.5 m = 1

/*
	r = 10;
	a = 0.5;
	m = 1;
	s[0] = 0;
	s[1] = 0;
	s[2] = 1;

*/
	//configuration 5
	//s = [0,0,1] r = 100 a = 0.5 m = 1

/*
	r = 100;
	a = 0.5;
	m = 1;
	s[0] = 0;
	s[1] = 0;
	s[2] = 1;
*/

	//configuration 6 
	//s = [0,0,1] r = 50 a = 0.1 m = 1

/*
	r = 50;
	a = 0.1;
	m = 1;
	s[0] = 0;
	s[1] = 0;
	s[2] = 1;
*/

	//configuration 7 
	//s = [0,0,1] r = 50 a = 1 m = 1
/*

	r = 50;
	a = 1;
	m = 1;
	s[0] = 0;
	s[1] = 0;
	s[2] = 1;
*/

	//configuration 8 
	//s = [0,0,1] r = 50 a = 0.5 m = 0.1

/*
	r = 50;
	a = 0.5;
	m = 0.1;
	s[0] = 0;
	s[1] = 0;
	s[2] = 1;
*/

	//configuration 9 
	//s = [0,0,1] r = 50 a = 0.5 m = 10000


	r = 50;
	a = 0.5;
	m = 10000;
	s[0] = 0;
	s[1] = 0;
	s[2] = 1;




	v[0] = 0;
	v[1] = 0;
	v[2] = 1;
	
	for(i = 0;i <3; i++){
		h[i] = v[i] + s[i];
	
	}
	
	hn = sqrt( pow(h[0],2) + pow(h[1],2) + pow(h[2],2) );

	for(i = 0;i<3;i++){
		h[i] = h[i]/hn;

	}

	for(i=0; i< ROWS; i++){
		for(j=0;j< COLS;j++){
			
			
				y = midy-i;
				x = j-midx;
				if((x*x +y*y) <= r*r){
				n[0] = x/r;
				n[1] = y/r;
				n[2] = ( sqrt( r*r - (x*x + y*y)) )/r;
				
				nds = s[0]*n[0] + s[1]*n[1] + s[2]*n[2];
				
				
						
				
				L_l = nds;		
			

		
				if( L_l >= 0){	
				
					
				
					hdn = h[0]*n[0] + h[1]*n[1] + h[2]*n[2];
				
					alpha = acos(hdn);
					am = pow(alpha/m,2) ;
					L_s = exp(-am);
			
					L = (a*L_l) + ( (1-a)*L_s );
					if(L > 1){
						printf("\nL = %f",L);
					}
			
					
					image[i][j] = L*255;
				}
			
			
				}
				
		}
	}



	if (!( fp = fopen( filename, "wb" ) ))
	{
	  fprintf( stderr, "error: could not open %s\n", filename );
	  exit( 1 );
	}
	fwrite( head, 4, 8, fp );
	for(i = 0; i<ROWS; i++){
		fwrite(image[i],1,COLS,fp);
	}		

	fclose( fp );
    return 0;

}



void clear( unsigned char image[ROWS][COLS] )
{
	int	i,j;
	for ( i = 0 ; i < ROWS ; i++ )
		for ( j = 0 ; j < COLS ; j++ ) image[i][j] = 0;
}

void header( int row, int col, unsigned char head[32] )
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify this */
	/* Little-endian for PC */
	
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch ++; 
	head[6] = *ch;
	ch ++;
	head[5] = *ch;
	ch ++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch ++; 
	head[10] = *ch;
	ch ++;
	head[9] = *ch;
	ch ++;
	head[8] = *ch;
	
	ch = (char*)&num;
	head[19] = *ch;
	ch ++; 
	head[18] = *ch;
	ch ++;
	head[17] = *ch;
	ch ++;
	head[16] = *ch;
	

	/* Big-endian for unix */
	/*
	*p = 0x59a66a95;
	*(p + 1) = col;
	*(p + 2) = row;
	*(p + 3) = 0x8;
	*(p + 4) = num;
	*(p + 5) = 0x1;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8;
*/
}


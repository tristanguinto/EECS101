#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define ROWS		(int)480
#define COLUMNS		(int)640

void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );

int main( int argc, char **argv )
{

	int				i, j, k, h, l, threshold[3], max[3];
	FILE			*fp;
	int 	hold[3][3];
	double x,y,sx,sy, s;
	double mx = 0;
	double my = 0;
	double ms = 0;
	double sdouble[ROWS][COLUMNS];
	unsigned char	image[ROWS][COLUMNS], ximage[ROWS][COLUMNS], yimage[ROWS][COLUMNS], simage[ROWS][COLUMNS],bimage[ROWS][COLUMNS], head[32];
	char			filename[6][50], ifilename[50], ch;

	strcpy( filename[0], "image1" );
	strcpy( filename[1], "image2" );
	strcpy( filename[2], "image3" );
	header ( ROWS, COLUMNS, head );

	printf( "Maximum of Gx, Gy, SGM\n" );


	threshold[0] = 140;
	threshold[1] = 154;
	threshold[2] = 156;
	for ( k = 0; k < 3; k ++)
	{
		clear( ximage );
		clear( yimage );
		mx = 0;
		my = 0;
		ms = 0;
		/* Read in the image */
		strcpy( ifilename, filename[k] );
		if (( fp = fopen( strcat(ifilename, ".raw"), "rb" )) == NULL )
		{
		  fprintf( stderr, "error: couldn't open %s\n", ifilename );
		  exit( 1 );
		}			
		for ( i = 0; i < ROWS ; i++ )
		  if ( fread( image[i], sizeof(char), COLUMNS, fp ) != COLUMNS )
		  {
			fprintf( stderr, "error: couldn't read enough stuff\n" );
			exit( 1 );
		  }
		fclose( fp );

		max[0] = 0; //maximum of Gx
		max[1] = 0; //maximum of Gy
		max[2] = 0; //maximum of SGM

		/*basic grey level image.ras*/


		
		strcpy(ifilename, filename[k]);
		if(!( fp = fopen( strcat(ifilename, "-g.ras"), "wb"))){
			fprintf(stderr, "error: could not open %s\n", ifilename);
			exit(1);
		}
	
		fwrite(head,4,8,fp);
		for(i = 0; i< ROWS; i++){
			fwrite( image[i] , 1, COLUMNS, fp);
		}
		fclose(fp);

		/* Compute Gx, Gy, SGM, find out the maximum and normalize*/
		
		//need to ignore x = 0, y = 0, x = 479, y = 639
		

		for(i = 0; i< ROWS; i++){
			for(j = 0; j<COLUMNS;j++){
				ximage[i][j] = image[i][j];
				yimage[i][j] = image[i][j];
				simage[i][j] = image[i][j];
			}
		}


		for(i = 1; i < ROWS - 1; i++){
			for(j = 1; j < COLUMNS -1; j++){
				
				/* de/dx */
				x = 0;
				hold[0][0] =  -1 * image[i-1][j-1];
				hold[0][1] =   0 * image[i-1][j];
				hold[0][2] =       image[i-1][j+1];

				hold[1][0] =  -2 * image[i][j-1];
				hold[1][1] =   0 * image[i][j];
				hold[1][2] =   2 * image[i][j+1];
				
				hold[2][0] =  -1 * image[i+1][j-1];
				hold[2][1] =   0 * image[i+1][j];
				hold[2][2] =       image[i+1][j+1];

				for(h = 0; h<3; h++){
					for(l = 0; l<3; l++){
					
						x = x + hold[h][l];
					}
					
				}
			
		
				if(x > mx){
					mx = x;

				}
				ximage[i][j] = abs(x);


				
				/* de/dy */
				y = 0;
				hold[0][0] =  -1 * image[i-1][j-1];
				hold[0][1] =  -2 * image[i-1][j];
				hold[0][2] =  -1 * image[i-1][j+1];

				hold[1][0] =   0 * image[i][j-1];
				hold[1][1] =   0 * image[i][j];
				hold[1][2] =   0 * image[i][j+1];
				
				hold[2][0] =       image[i+1][j-1];
				hold[2][1] =   2 * image[i+1][j];
				hold[2][2] =       image[i+1][j+1];

				for(h = 0; h<3; h++){
					for(l = 0; l<3; l++){
					
						y = y + hold[h][l];
					}
					
				}

		
				if(y > my){
					my = y;

				}
				yimage[i][j] = abs(y);




				sx = x*x;
				sy = y*y;
				s = sx + sy;
				sdouble[i][j] = s;
				if(s > ms){
					ms = s;
				}
			}
		}
		

	
		
		printf("\nFor Image %d",k+1);
		printf("\nmx is = %f", mx);
		printf("\nmy is = %f", my);
		printf("\nms is = %f", ms);

		for(i = 0; i< ROWS; i++){
			for(j = 0; j<COLUMNS;j++){
				ximage[i][j] = (((ximage[i][j]) * 255) / mx);
			
				yimage[i][j] = (((yimage[i][j]) * 255) / my);
		
				s = sdouble[i][j];
				simage[i][j] = s*255/ms;
			}
		}


//Binary Image


		for(i = 0; i < ROWS; i++){
			for(j = 0; j < COLUMNS; j++){
				if(k==0){	
					if( image[i][j]  >=threshold[0]){
						bimage[i][j] = 0;
					}
					else if( image[i][j] < threshold[0]){
						bimage[i][j]  = 255;
					}
				}
				else if(k==1){
					if(image[i][j] >=threshold[1]){
						bimage[i][j] = 0;
					}
					else if( image[i][j] <threshold[1]){
						bimage[i][j] = 255;
					}
				}
				else if(k==2){
					if(image[i][j]>=threshold[2]){
						bimage[i][j] = 0;
					}
					else if(image[i][j]<threshold[2]){

						bimage[i][j] = 255;
					}
				}
			}
		
		}

		for( i = 1; i<ROWS-1; i++){
			for(j = 1; j< COLUMNS-1; j++){
				if(bimage[i-1][j] == 0){
					if(bimage[i][j-1] ==0){
						if(bimage[i+1][j] == 0 || bimage[i][j+1] == 0){
								bimage[i][j] = 0;
						}
					}
				}
			}
		}
		
		for( i = 1; i<ROWS-1; i++){
			for(j = 1;j<COLUMNS-1;j++){
				if(bimage[i-1][j] == 255 || bimage[i][j-1] == 255){
					if(bimage[i+1][j]==255 || bimage[i][j+1] ==255){ 
						bimage[i][j] = 255;
					}
				}
			}
		}
		/* Write Gx to a new image*/
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-x.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for(i = 0; i<ROWS; i++){
			fwrite(ximage[i],1,COLUMNS,fp);
		}		
		
		
		fclose( fp );
					
		/* Write Gy to a new image */
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-y.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for(i = 0; i<ROWS; i++){
			fwrite(yimage[i],1,COLUMNS,fp);
		}		
	
		fclose( fp );

		

		/* Write SGM to a new image */
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-s.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for(i = 0; i<ROWS;i++){
			fwrite(simage[i],1,COLUMNS,fp);
		}
			
	
		fclose( fp );
		
		
		/* Compute the binary image */
		
		/* Write the binary image to a new image */
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-b.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for(i = 0; i<ROWS;i++){
			fwrite(bimage[i],1,COLUMNS,fp);
		}
		fclose( fp );
		max[0] = mx;
		max[1] = my;
		max[2] = ms;
		printf( "\n%d %d %d\n", max[0], max[1], max[2] );

	}

	printf( "Press any key to exit: " );
	gets( &ch );
	return 0;
}

void clear( unsigned char image[][COLUMNS] )
{
	int	i,j;
	for ( i = 0 ; i < ROWS ; i++ )
		for ( j = 0 ; j < COLUMNS ; j++ ) image[i][j] = 0;
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

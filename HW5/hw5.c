#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS	480
#define COLS	640
#define PI      3.1415927
#define sqr(x)	((x)*(x))

void clear( unsigned char image[ROWS][COLS] );
void header( int row, int col, unsigned char head[32] );

int main( int argc, char** argv )
{
	int				i,j,h,p;
	double ms = 0;
	double s;
	int				dedx, dedy, sgm;
	int				sgm_threshold, hough_threshold, voting[180][1600];
	FILE*			fp;
	double sdouble[ROWS][COLS];
	unsigned char	himage[ROWS][COLS], e[ROWS][COLS], image[ROWS][COLS],simage[ROWS][COLS],gimage[ROWS][COLS],head[32];
	char			filename[50], sfilename[50],gfilename[50],efilename[50],hfilename[50];
	header(ROWS,COLS,head);
	
	strcpy ( filename, "image.raw" );
	strcpy(sfilename,"simage.ras");
	strcpy(gfilename,"gimage.ras");
	strcpy(efilename,"eimage.ras");
	strcpy(hfilename,"himage.ras");
	memset ( voting, 0, sizeof(int) * 180 * 400 );
	

	sgm_threshold = 144;

	if (!( fp = fopen( filename, "rb" ) ))
	{
		fprintf( stderr, "error: couldn't open %s\n", argv[1] );
		exit(1);
	}

	for ( i = 0 ; i < ROWS ; i++ )
		if (!( COLS == fread( image[i], sizeof(char), COLS, fp ) ))
		{
			fprintf( stderr, "error: couldn't read %s\n", argv[1] );
			exit(1);
		}
	fclose(fp);


	for(i = 0; i <ROWS; i++){
		for(j=0;j<COLS; j++){
			simage[i][j] = image[i][j];
			gimage[i][j] = image[i][j];
			sdouble[i][j] = 0;	
			himage[i][j] = 0;
			e[i][j] = 0;
		}
	}

	for ( i = 1 ; i < ROWS -1 ; i++ ){
		for ( j = 1 ; j < COLS - 1 ; j++ )
		{
			dedx =
					  -image[i-1][j-1] +   image[i-1][j+1] +
					-2*image[ i ][j-1] + 2*image[ i ][j+1] +
					  -image[i+1][j-1] +   image[i+1][j+1] ;

			dedy =
				-image[i-1][j-1] - 2*image[i-1][ j ] - image[i-1][j+1] +
				 image[i+1][j-1] + 2*image[i+1][ j ] + image[i+1][j+1] ;
			s = sqr(dedx) + sqr(dedy);
			sgm = sqr(dedx) + sqr(dedy);
			sdouble[i][j] = s;
			simage[i][j] = sgm;
			if(ms < s){
				ms = s;
			
			}
		}
	}

	//limit from 0 - 255

	for(i = ROWS;i<0;i--){
		for(j=COLS;j<0;j--){
			s = sdouble[i][j];
			simage[i][j] = (s*255)/ms;
		}
	}			
	

	//threshold edge values for binary image





	for(i = 1;i<ROWS-1;i++){
		for(j = 1;j<COLS-1;j++){
			if(simage[i][j] >= sgm_threshold){
				e[i][j] = 255;
			} else if(simage[i][j] < sgm_threshold){
				e[i][j] = 0;
			}
			
		}
	}


	//calculate theta and rho for every x,y
	//offset by 800, 800 is new '0' of rho
	for(i = 0;i < 180;i++){
		for(j = 0; j<1600; j++){
			voting[i][j] = 0;
		}
	}


	for(i=0;i<ROWS;i++){
		for(j=0;j<COLS;j++){
			if(e[i][j] == 255){
				sgm = simage[i][j];
				for(h=0;h<180;h++){
					p = i*cos(h*PI/180) - j*sin(h*PI/180);
					voting[h][p+800] = voting[h][p+800] + 1;
				}													
			}			
		}
	}	


	
	for(i=179;i>0;i--){
		for(j = 0; j<1600; j++){
			if(voting[i][j] >130 && voting[i][j] <500){
					
				printf("theta: %d + rho: %d + vote: %d\n",i,j-800,voting[i][j]);
			}
		}
		
	}
	//found by finding local maximas directly from printed value of voting array
	hough_threshold = 130;


	//find where hough equation is equal to 0 and plot white pixels
	
	for(i = ROWS - 1;i>0;i--){
		for(j = 0;j<COLS;j++){
			if( (int)(j*sin(51*PI/180) - i*cos(51*PI/180) - 170) == 0){
				himage[i][j] = 255;
			}
			if( (int)(j*sin(14*PI/180) - i*cos(14*PI/180) + 313) == 0){
				himage[i][j] = 255;
			}
			if( (int)(j*sin(129*PI/180) - i*cos(129*PI/180) - 294) == 0){
				himage[i][j] = 255;
			}

					
		}
	}

	if (!( fp = fopen( sfilename, "wb" ) ))
	{
	  fprintf( stderr, "error: could not open %s\n", sfilename );
	  exit( 1 );
	}
	fwrite( head, 4, 8, fp );
	for(i = 0; i<ROWS; i++){
		fwrite(simage[i],1,COLS,fp);
	}		

	fclose( fp );

	if (!( fp = fopen( gfilename, "wb" ) ))
	{
	  fprintf( stderr, "error: could not open %s\n", sfilename );
	  exit( 1 );
	}
	fwrite( head, 4, 8, fp );
	for(i = 0; i<ROWS; i++){
		fwrite(gimage[i],1,COLS,fp);
	}		
	fclose( fp );

	if(!(fp = fopen(efilename,"wb"))){
		fprintf(stderr,"error");
		exit(1);
	}
	fwrite(head,4,8,fp);
	for(i=0;i<ROWS;i++){
		fwrite(e[i],1,COLS,fp);
	}
	fclose(fp);

	if(!(fp = fopen(hfilename,"wb"))){
		fprintf(stderr,"error");
		exit(1);
	}
	fwrite(head,4,8,fp);
	for(i=0;i<ROWS;i++){
		fwrite(himage[i],1,COLS,fp);
	}
	fclose(fp);

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


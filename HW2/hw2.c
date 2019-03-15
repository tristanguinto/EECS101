#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ROWS		100
#define COLUMNS		100

int main( int argc, char **argv )
{

	int			i,j;
	FILE			*fp, *pfile;
	float			u, var;
	unsigned char	image[ROWS][COLUMNS];
  	char			*ifile, *ofile;
	char ch[1];
	char			filename[4][50];
	float n = 100;
	u = 0;
	var = 0;
	strcpy(filename[0], "image1.raw");
	strcpy(filename[1], "image2.raw");
	strcpy(filename[2], "image3.raw");
	strcpy(filename[3], "image4.raw");




	//Assign each image name in filename to ifile here	
	/* example: ifile = filename[k]; k=0,1,2,3; a loop might be needed*/
	int k;
	for( k = 0; k<4; k++){
	
	ifile = filename[k];
	printf("Attempting to read file\n");

	
	if (( fp = fopen( ifile, "rb" )) == NULL )
	{
	  fprintf( stderr, "error: couldn't open %s\n", ifile );
	  exit( 1 );
	}
	
	printf("File read\n");
	
	for ( i = 0; i < ROWS ; i++ )
	  if ( fread( image[i], 1, COLUMNS, fp ) != COLUMNS )
	  {
	    fprintf( stderr, "error: couldn't read enough stuff\n" );
	    exit( 1 );
	  }
	
	fclose( fp );
	//Calculate Mean for each image here
	
	for( i = 0; i < ROWS; i++){
		for (j = 0; j<COLUMNS; j++){
			u += image[i][j];
		}
	}
	u = u/(n * n);

	//Calculate Variance for each image here

	for( i = 0; i < ROWS; i++){
		for (j = 0; j<COLUMNS; j++){
			var += (image[i][j] - u) * (image[i][j] - u);
		}
	}
	var = var/( (n * n) -1);

	//Print mean and variance for each image
	printf("%s: %f %f\n", ifile, u, var);
	}
	printf("Press any key to exit: ");
	while(1){
		scanf("%s", &ch);
		if(ch != NULL) break;
	}
	return 0;
}



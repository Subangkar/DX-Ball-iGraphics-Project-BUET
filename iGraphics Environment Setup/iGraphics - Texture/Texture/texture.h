#ifndef _LOAD_BMP
#define _LOAD_BMP

#define BMP_RGB  0
#define BMP_RGBA 1

typedef struct _image 
{
	int width ;
	int height ;
	int r,g,b;
	unsigned char *data ;
	int type ;
} IMAGE ;

extern void LoadBMP( IMAGE *image, char * filename);
extern void LoadBMPWithAlpha(IMAGE *image, char *filename);

struct texture
{
	unsigned int  texName ;
	IMAGE img;
	void Create( char *filename  );
	void Draw( ) ;

} ;


#endif
#ifndef LIB_BMP
#define LIB_BMP

#include <intTypes.h>

typedef struct bmp_header_struct
{
	uint8_t bmpIdentifier[2]; //Alway 'BM'. Integrity check if file is actually BMP.
	uint32_t bmpSize;
	uint32_t bmpReserved;
	uint32_t bmpImageOffset;
	
	uint32_t bmpHeaderSize;
	int bmpWidth;
	int bmpHeight;
	uint16_t bmpPlanes;
	uint16_t bmpColourDepth; //16 is RGB565 which we have used
 	uint32_t bmpCompressionType; //0 is no compression. Should be 0 as compressed bmp's are not supported in this lib
	uint32_t bmpImageSize;
	int bmpXPixelsPerMeter;
	int bmpYPixelsPerMeter;
	uint32_t bmpColourUsed;
	uint32_t bmpColourImportant;
} bmp_header_t;

void renderBmp(uint8_t* file, uint32_t fileSize, uint16_t x, uint16_t y);

#endif

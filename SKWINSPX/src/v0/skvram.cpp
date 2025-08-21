//------------------------------------------------------------------------------

#include <sktypes.h>
#include <skvram.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------


typedef X8 C3[3];

// EGA colors (0-15)
C3 xEGAPalette[16] = {
    {  0,   0,   0},     // 0: Black
    {  0,   0, 170},     // 1: Blue
    {  0, 170,   0},     // 2: Green
    {  0, 170, 170},     // 3: Cyan
    {170,   0,   0},     // 4: Red
    {170,   0, 170},     // 5: Magenta
    {170,  85,   0},     // 6: Brown / Dark Yellow
    {170, 170, 170},     // 7: Light Gray
    { 85,  85,  85},     // 8: Dark Gray
    { 85,  85, 255},     // 9: Light Blue
    { 85, 255,  85},     // 10: Light Green
    { 85, 255, 255},     // 11: Light Cyan
    {255,  85,  85},     // 12: Light Red
    {255,  85, 255},     // 13: Light Magenta
    {255, 255,  85},     // 14: Yellow
    {255, 255, 255}      // 15: White
};

//------------------------------------------------------------------------------

SkVRAM::SkVRAM()
{
	VRAM_SCREEN_WIDTH = 320;
	VRAM_SCREEN_HEIGHT = 200;

	VRAM = (X8*) calloc (VRAM_SCREEN_WIDTH * VRAM_SCREEN_HEIGHT, sizeof(X8));
	xVGAPalette = (X8*) calloc (256 * 4, sizeof(X8));

	INIT_VGA_PALETTE();
}

SkVRAM::~SkVRAM()
{
	free(VRAM);
	free(xVGAPalette);
}

X8* SkVRAM::GET_VIDEO_ARRAY()
{
	return VRAM;
}

X8* SkVRAM::GET_PALETTE()
{
	return xVGAPalette;
}

//------------------------------------------------------------------------------

UINT SkVRAM::SET_TEST_VRAM_STATIC()
{
	UINT x, y, z;
    for (y = 0; y < VRAM_SCREEN_HEIGHT; y++) {
		z = y * VRAM_SCREEN_WIDTH;
        for (x = 0; x < VRAM_SCREEN_WIDTH; x++) {
			VRAM[z] = (X8) (rand()%255);
			z++;
        }
    }
	return 0;
}

UINT SkVRAM::SET_TEST_VRAM_GRADIENT()
{
	UINT x, y, z;
    // Fill screen with a gradient
    for (y = 0; y < VRAM_SCREEN_HEIGHT; y++) {
		z = y * VRAM_SCREEN_WIDTH;
        for (x = 0; x < VRAM_SCREEN_WIDTH; x++) {
			VRAM[z] = (X8)((x + y) % 256);
			z++;
        }
    }
	return 0;
}


UINT SkVRAM::INIT_GREYSCALE_PALETTE()
{
    for (int i = 0; i < 256; i++) {
        xVGAPalette[i * 4 + 0] = i;
        xVGAPalette[i * 4 + 1] = i;
        xVGAPalette[i * 4 + 2] = i;
		xVGAPalette[i * 4 + 3] = 0;
    }
	return 0;
}

UINT SkVRAM::COPY_VRAM_DATA(X8* xSourceVRAMData)
{
	void* rc = NULL;
	rc = memcpy(VRAM, xSourceVRAMData, 320*200);
//	printf("MEMCOPY RC = %08x\n", rc);
	
	// SPECIAL PALETTE
	/*
	for (UINT j = 0; j < 8; j++) {
		for (UINT i = 0; i < 256; i++)
			VRAM[i+j*320] = i;
	}*/
	
	return 0;
}

UINT SkVRAM::INIT_VGA_PALETTE()
{
    INIT_GREYSCALE_PALETTE();
	for (UINT i = 0; i < 16; i++) { // EGA
		xVGAPalette[i * 4 + 0] = xEGAPalette[i][0];
		xVGAPalette[i * 4 + 1] = xEGAPalette[i][1];
		xVGAPalette[i * 4 + 2] = xEGAPalette[i][2];
		xVGAPalette[i * 4 + 3] = 0;
	}
	return 0;
}

UINT SkVRAM::SET_VGA_PALETTE_BMP8(X8 *xBMPPalette)
{
    for (int i = 0; i < 256; i++) {
        xVGAPalette[i * 4 + 0] = xBMPPalette[i * 4 + 2];  // Red
        xVGAPalette[i * 4 + 1] = xBMPPalette[i * 4 + 1];  // Green
        xVGAPalette[i * 4 + 2] = xBMPPalette[i * 4 + 0];  // Blue
		xVGAPalette[i * 4 + 3] = 0;
    }
	return 0;
}

UINT SkVRAM::SET_VGA_PALETTE_3RGB(X8 *xRGBPalette)
{
    for (int i = 0; i < 256; i++) {
        xVGAPalette[i * 4 + 0] = xRGBPalette[i * 3 + 0];
        xVGAPalette[i * 4 + 1] = xRGBPalette[i * 3 + 1];
        xVGAPalette[i * 4 + 2] = xRGBPalette[i * 3 + 2];
		xVGAPalette[i * 4 + 3] = 0;
    }
	return 0;
}

UINT SkVRAM::SET_VGA_PALETTE_3RGB6(X8 *xRGBPalette)
{
    for (int i = 0; i < 256; i++) {
        xVGAPalette[i * 4 + 0] = (xRGBPalette[i * 3 + 0]*4);
        xVGAPalette[i * 4 + 1] = (xRGBPalette[i * 3 + 1]*4);
        xVGAPalette[i * 4 + 2] = (xRGBPalette[i * 3 + 2]*4);
		xVGAPalette[i * 4 + 3] = 0;
    }
	return 0;
}


UINT SkVRAM::SET_TEST_TITLE()
{
	X8* xImageData = NULL;
	//UINT iNbPixels = VRAM_SCREEN_HEIGHT * VRAM_SCREEN_WIDTH;
	X8 xPalette[4*256];

	// Read a 8-bit bmp image of Skullkeep title as a test
	// The image is 320 * 175
    {
		FILE *file = fopen("dm2sk.raw", "rb");
		if (!file) {
			perror("Error opening file dm2sk.raw");
			return 1;
		}

		fseek(file, 0x32, SEEK_SET);

		// Read palette (256 x RGB)
		if (fread(xPalette, 1, 4*256, file) != (4*256)) { 
			fprintf(stderr, "Error reading palette data.\n");
			fclose(file);
			return 1;
		}
		SET_VGA_PALETTE_BMP8(xPalette);

		xImageData = (X8*) calloc(VRAM_SCREEN_WIDTH*VRAM_SCREEN_HEIGHT, sizeof(X8));

		if (fread(xImageData, 1, 320 * 175, file) != (320 * 175)) {
			fprintf(stderr, "Error reading image data.\n");
			fclose(file);
			return 1;
		}
	}

	memset(VRAM, 0, 320 * 200);
	memcpy(VRAM+(12*VRAM_SCREEN_WIDTH), xImageData, 320 * 175);

	free(xImageData);
	return 0;
}

//------------------------------------------------------------------------------

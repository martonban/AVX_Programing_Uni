#include <stdio.h>
#include <immintrin.h>

#define ORIGINALSIZE1 1080000
#define ORIGINALSIZE2 4320000
#define VERTICALSIZE 2160000
#define HEADER 54


int main(){

	// A KÉPEK BETÖLTÉSE
	// Az alap képek megnyítása
	FILE *spaceOriginal1 = fopen("space_1.bmp", "rb");
	FILE *spaceOriginal2 = fopen("space_2.bmp", "rb");
	// Az általunk elkészített kép
	FILE *result = fopen("space_new.bmp", "wb+");

	// A képek helyének a memória foglalása
	char *header = (char*) malloc(HEADER*sizeof (char));
	char *header1 = (char*) malloc(HEADER*sizeof (char));
	char *originData1 = (char*) malloc(ORIGINALSIZE1*sizeof (char));
	char *originData2 = (char*) malloc(ORIGINALSIZE2*sizeof (char));


	// A kép adatainak betöltése az előkészített memóriába.
	for(int i=0; i<HEADER; i++) {
		header[i] = fgetc(spaceOriginal2);
	}

	for(int i=0; i<HEADER; i++){
		header[i] = fgetc(spaceOriginal1);
	}


	for(int i=0; i<ORIGINALSIZE1; i++){
		originData1[i] = fgetc(spaceOriginal1);
	}

	for(int i=0; i<ORIGINALSIZE2; i++){
		originData2[i] = fgetc(spaceOriginal2);
	}


	// A KÉP CSÖKKENTÉSE
	// Vertikálisan átlagolunk.
	// Result memória foglalás
	char *resultData = (char*) malloc((ORIGINALSIZE2)*sizeof (char));
	int j = 0;
	int r = 0;

	// Vertikális értékek átlagolása
	for (int i = 0; i<ORIGINALSIZE2; i += 32, j += 32, r++){
		__m256i mm_a = _mm256_loadu_si256((__m256i *)&originData2[i]);
		__m256i mm_b = _mm256_loadu_si256((__m256i *)&originData2[i+3600]);
		__m256i mm_c = _mm256_avg_epu8(mm_a, mm_b);
		_mm256_storeu_si256((__m256i *)&resultData[j], mm_c);
		if(r>=112){
			r = 0;
			i += 3600;
		}
	}


	// HORIZÁNTÁLIS PIXEL ELDOBÁS
	char *resultPixelData = (char*)malloc((ORIGINALSIZE1)*sizeof (char));
	for(int i = 0 , j = 0; j<ORIGINALSIZE2; i+=3, j+=6){
		for(int z = 0; z<5; z++ ){
			resultPixelData[i+z] = resultData[j+z];
		}
	}

	free(originData2);

	char *finalResult = (char*)malloc((ORIGINALSIZE1)*sizeof (char));
	for(int i = 0; i<ORIGINALSIZE1; i+=32){
		__m256i mm_a = _mm256_loadu_si256((__m256i *)&resultPixelData[i]);
                __m256i mm_b = _mm256_loadu_si256((__m256i *)&originData1[i]);
                __m256i mm_c = _mm256_avg_epu8(mm_a, mm_b);
                _mm256_storeu_si256((__m256i *)&finalResult[i], mm_c);
	}


	//Az eredmény kiiratása
	for(int i=0; i<HEADER; i++){
		fputc(header[i], result);
	}

	for(int i=0; i<ORIGINALSIZE1; i++) {
		fputc(finalResult[i], result);
	}


	return 0;
}

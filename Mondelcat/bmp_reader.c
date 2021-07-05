
#include "bmp_reader.h"

char* load_bitmap_file(char* filename, s_bitmap_info_header* header) {
    s_bitmap_file_header bitmapFileHeader = {0};

    FILE* filePtr = fopen(filename, "rb");
    if(filePtr == NULL) return NULL;

    fread(&bitmapFileHeader, sizeof(s_bitmap_file_header), 1, filePtr);

    if(bitmapFileHeader.bfType != 0x4D42) {
        fclose(filePtr);
        return NULL;
    }

    fread(header, sizeof(s_bitmap_info_header), 1, filePtr);
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    char* bitmapImage = (char*) calloc(buffer_size(header->biWidth, header->biHeight), sizeof(char));

    if(!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    fread(bitmapImage, 1, header->biSizeImage, filePtr);

    if(bitmapImage == NULL) {
        fclose(filePtr);
        return NULL;
    }

    if(header->biHeight < 0) {
        header->biHeight = -header->biHeight;

        bmp_flip_image_vertical(bitmapImage, header);
    }

    fclose(filePtr);
    return bitmapImage;
}

void bmp_flip_image_vertical(char* bitmapImage, s_bitmap_info_header* header) {
    int width = header->biWidth;
    int height = header->biHeight;

    for(int j = 0, j2 = height - 1; j < j2; j++, j2--) {
        int j_base = j * width * 4;
        int j2_base = j2 * width * 4;

        for(int i = 0, i_base = 0; i < width; i++, i_base += 4) {
            int tmp = *(int*)((bitmapImage + j_base + i_base));
            *(int*)(bitmapImage + j_base + i_base) = *(int*)(bitmapImage + j2_base + i_base);
            *(int*)(bitmapImage + j2_base + i_base) = tmp;
        }
    }
}

void bmp_flip_image_horizontal(char* bitmapImage, s_bitmap_info_header* header) {
    int width = header->biWidth;
    int height = header->biHeight;

    for(int j = 0; j < height; j++) {
        int j_base = j * width * 4;

        for(int i = 0, i_base = 0, i2_base = (width - 1) * 4; i_base < i2_base; i++, i_base += 4, i2_base -= 4) {
            int tmp = *(int*)((bitmapImage + j_base + i_base));
            *(int*)(bitmapImage + j_base + i_base) = *(int*)(bitmapImage + j_base + i2_base);
            *(int*)(bitmapImage + j_base + i2_base) = tmp;
        }
    }
}

long long buffer_size(long long width, long long height) {
    long long result = llabs(width) * llabs(height) * 4;
    if(result & 15) {
        result += 16;
        result &= ~15;
    }
    result += 16;
    return result;
}
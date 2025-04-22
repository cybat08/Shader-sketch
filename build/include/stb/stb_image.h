
// Minimal stb_image.h stub
#ifndef STB_IMAGE_H
#define STB_IMAGE_H

#include <stdlib.h>

unsigned char *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
void stbi_image_free(void *retval_from_stbi_load);

// implementation
unsigned char *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels) {
    *x = 1; *y = 1; *channels_in_file = 4;
    return (unsigned char*)malloc(4);
}

void stbi_image_free(void *retval_from_stbi_load) {
    free(retval_from_stbi_load);
}

#endif // STB_IMAGE_H

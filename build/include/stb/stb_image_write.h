
// Minimal stb_image_write.h stub
#ifndef STB_IMAGE_WRITE_H
#define STB_IMAGE_WRITE_H

int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);

// implementation
int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes) {
    return 1;
}

#endif // STB_IMAGE_WRITE_H

#ifndef API_H
#define API_H

#define HEIGHT_NUM 400
#define WIDTH_NUM 500

#ifdef __cplusplus
extern "C" {
#endif

int generate();
void init(unsigned int width, unsigned int height);
void flush();
void setPixel(unsigned int x, unsigned int y, int is_alive);
void display();

#ifdef __cplusplus
}
#endif

#endif  // API_H
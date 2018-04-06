void box_filtering_TAD_stereo_color(int w, int h, int nDisp, int offs_x, IplImage *L, IplImage *R, int** out, int r, int threshold);

void park_box_filtering_TAD_stereo_color(int w, int h, int nDisp, int offs_x, IplImage *L, IplImage *R, int** out, int r, int threshold, int census_size);

int StandardStereo_DSI(IplImage *Left, IplImage *Right, int dmin, int dmax, int radius, int TAD_threshold, int *output_DSI);

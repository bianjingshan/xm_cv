
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "xm_text.h"
#include "xm_image.h"
#include "time_utils.h"




int main(int argc, char *argv[]) {
    XMText xm_text;
    xm_text.init("MSYH.TTF");
    xm_text.setFont(50);

    Mat image;
    CreateImage(image, Size(1280, 720), 3);
    MatSet(image, g_black);
    TickTime timer;
    timer.start();
    xm_text.putTextMat(image, Point(100, 100), "这是一段中文。Hello world. 2020-03-06");
    printf("%f ms\n", timer.timerMsec());

    timer.start();
    ShowText(image, Point(100, 400), "This is opencv putText, 2020-03-06", g_white, 1, 2);
    printf("%f ms\n", timer.timerMsec());
    imshow("image", image);
    waitKey(-1);
    return 0;
}

#include <stdio.h>
#include <GL/glx.h>

int main()
{
    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL)
    {
        printf("无法打开X Display\n");
        return 1;
    }

    GLXQueryVersion *queryVersion = glXQueryVersion(dpy);
    if (queryVersion)
    {
        printf("系统有GPU，GLX版本：%d.%d\n", queryVersion->major, queryVersion->minor);
    }
    else
    {
        printf("系统没有GPU或者无法查询GLX版本\n");
    }

    XCloseDisplay(dpy);
    return 0;
}

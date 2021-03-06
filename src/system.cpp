#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

#include "console.h"
#include "engine.h"
#include "system.h"
#include "script.h"
#include "gl_util.h"
#include "gui.h"


ScreenInfo screen_info;
gui_text_line_t system_fps;


// =======================================================================
// General routines
// =======================================================================

void Sys_Printf(char *fmt, ...)
{
    va_list     argptr;
    char        text[4096];

    va_start (argptr,fmt);
    vsnprintf (text, 4096, fmt,argptr);
    va_end (argptr);
    fprintf(stderr, "%s", text);

    //Con_Print (text);
}

void Sys_Init()
{
    system_fps.text_size = 16;
    system_fps.text = (char*)malloc(system_fps.text_size * sizeof(char));
    system_fps.text[0] = 0;

    system_fps.X = (10.0);
    system_fps.Y = (10.0);
    system_fps.Xanchor = GUI_ANCHOR_HOR_RIGHT;
    system_fps.Yanchor = GUI_ANCHOR_VERT_BOTTOM;

    system_fps.font_id  = FONT_PRIMARY;
    system_fps.style_id = FONTSTYLE_MENU_TITLE;

    system_fps.show  = true;

    Gui_AddLine(&system_fps);
}

void Sys_InitGlobals()
{
    screen_info.x = 50;
    screen_info.y = 20;
    screen_info.w = 800;
    screen_info.h = 600;
    screen_info.FS_flag = 0;
    screen_info.show_debuginfo = 0;
    screen_info.fov = 75.0;
}

void Sys_Destroy()
{
    system_fps.show = false;
    system_fps.text_size = 0;
    free(system_fps.text);
    system_fps.text = NULL;
}

void Sys_Error(const char *error, ...)
{
    va_list     argptr;
    char        string[4096];

    va_start (argptr,error);
    vsnprintf (string, 4096, error, argptr);
    va_end (argptr);

    Sys_DebugLog(LOG_FILENAME, "System error: %s", string);
    Engine_Shutdown(1);
}

void Sys_Warn(const char *warning, ...)
{
    va_list     argptr;
    char        string[4096];

    va_start (argptr, warning);
    vsnprintf (string, 4096, warning, argptr);
    va_end (argptr);
    Sys_DebugLog(LOG_FILENAME, "Warning: %s", string);
}

void Sys_DebugLog(const char *file, const char *fmt, ...)
{
    va_list argptr;
    static char data[4096];
    FILE *fp;

    va_start(argptr, fmt);
    sprintf(data, "\n");
    vsnprintf(&data[1], 4095, fmt, argptr);
    va_end(argptr);
    fp = fopen(file, "a");
    if(fp == NULL)
    {
        fp = fopen(file, "w");
    }
    if(fp != NULL)
    {
        fwrite(data, strlen(data), 1, fp);
        fclose(fp);
    }
    fwrite(data, strlen(data), 1, stderr);
}

/*
===============================================================================

SYS TIME

===============================================================================
*/

void Sys_Strtime(char *buf, size_t buf_size)
{
    struct tm *tm_;
    static time_t t_;

    time(&t_);
    tm_=gmtime(&t_);

    snprintf(buf, buf_size, "%02d:%02d:%02d",tm_->tm_hour,tm_->tm_min,tm_->tm_sec);
}

void Sys_StrRunSec(char *buf, size_t buf_size)
{
    struct              timeval tp;
    static long int     secbase = 0;

    gettimeofday(&tp, NULL);

    if(!secbase)
    {
        secbase = tp.tv_sec;
    }

    snprintf(buf, buf_size, "%06d.%03d", (int)(tp.tv_sec-secbase), (int)(tp.tv_usec/1000));
}

btScalar Sys_FloatTime (void)
{
    struct              timeval tp;
    static long int     secbase = 0;

    gettimeofday(&tp, NULL);

    if (!secbase)
    {
        secbase = tp.tv_sec;
        return tp.tv_usec * 1.0e-6;
    }

    return (btScalar)(tp.tv_sec - secbase) + (btScalar)tp.tv_usec * 1.0e-6;
}

